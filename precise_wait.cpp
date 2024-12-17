#include "precise_wait.h"

#include"stop_watch.h"

// 正確なウェイト(標準なsleepは最大16msの誤差がある)
void PreciseWait( std::chrono::milliseconds ms )
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + ms;

    if ( ms > std::chrono::milliseconds( 20 ) )
    {
        std::this_thread::sleep_for( ms - std::chrono::milliseconds( 20 ) );
    }

    while ( std::chrono::high_resolution_clock::now() < end )
    {
        std::this_thread::yield();
    }
}

PreciseWaiter::PreciseWaiter()
{
    thread_ = std::jthread( [this]( std::stop_token st ) { ThreadFunc( st ); } );
}

PreciseWaiter::~PreciseWaiter()
{
    thread_.request_stop();
    cv_.notify_all();
    thread_.join();
}

void PreciseWaiter::PostTask( std::chrono::milliseconds ms, std::function< void() > task )
{
    auto now = std::chrono::high_resolution_clock::now();

    // 直前のタスクが実行されていない場合は破棄される
    auto lock = std::lock_guard< std::mutex >( mtx_ );
    end_ = now + ms;
    task_ = std::move( task );
    cv_.notify_all();
}

void PreciseWaiter::Abort()
{
    abort_ = true;
}

void PreciseWaiter::ThreadFunc( std::stop_token st )
{
    auto is_stop = [&] { return st.stop_requested(); };
    auto wait_task = [&]
    {
        auto lock = std::unique_lock< std::mutex >( mtx_ );
        cv_.wait( lock, [&] { return st.stop_requested() || task_ != nullptr; } );
    };
    auto is_end = [&]( std::chrono::time_point< std::chrono::high_resolution_clock > now )
    {
        auto lock = std::lock_guard< std::mutex >( mtx_ );
        return now >= end_;
    };
    auto is_abort = [&] { return abort_.load(); };

    while ( !is_stop() )
    {
        // task待機
        wait_task();
        StopWatchG::Wrap("wait_task ");

        // endまで待機
        while ( true )
        {
            auto now = std::chrono::high_resolution_clock::now();
            if ( is_stop() )
                return;
            if ( is_end( now ) || is_abort() )
                break;

            auto diff = end_ - now;
            if ( diff > std::chrono::milliseconds( 20 ) )
                std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
            else
                std::this_thread::yield();
        }
        StopWatchG::Wrap("wait end  ");

        // 状態に応じた処理
        auto lock = std::lock_guard< std::mutex >( mtx_ );
        if ( is_abort() )
        {
            abort_ = false;
            task_ = nullptr;
        }
        else
        {
            if ( task_ )
                task_();
            task_ = nullptr;
        }
        StopWatchG::Wrap("after task");
        StopWatchG::Print();
    }
}

HANDLE timer_g = CreateWaitableTimer( nullptr, FALSE, nullptr );
void WaitableWait( std::chrono::milliseconds ms )
{
    LARGE_INTEGER due_time;
    due_time.QuadPart = -static_cast< LONGLONG >( ms.count() ) * 1000 * 10; //100ns単位
    SetWaitableTimer( timer_g, &due_time, 0, nullptr, nullptr, FALSE );
    WaitForSingleObject( timer_g, INFINITE );
}

Waitable::Waitable()
    :abort_( false )
{
    timer_ = CreateWaitableTimer( nullptr, FALSE, nullptr );
    thread_ = std::jthread( [this]( std::stop_token st ) { ThreadFunc( st ); } );
}

Waitable::~Waitable()
{
    thread_.request_stop();
    LARGE_INTEGER due_time;
    due_time.QuadPart = -1;
    SetWaitableTimer( timer_, &due_time, 0, nullptr, nullptr, FALSE );
    thread_.join();
    CloseHandle( timer_ );
}

void Waitable::PostTask( std::chrono::milliseconds ms, std::function< void() > task )
{
    auto lock = std::lock_guard< std::mutex >(mtx_);
    
    CancelWaitableTimer(timer_);
    abort_ = false;
    task_ = std::move(task);
     
    LARGE_INTEGER due_time;
    due_time.QuadPart = -static_cast< LONGLONG >( ms.count() ) * 1000* 10; //100ns単位
    SetWaitableTimer( timer_, &due_time, 0, nullptr, nullptr, FALSE );
    StopWatchG::Wrap("set timer  ");
}

void Waitable::Abort()
{
    auto lock = std::lock_guard< std::mutex >(mtx_);

    abort_ = true;
    CancelWaitableTimer( timer_ );
}

void Waitable::ThreadFunc(std::stop_token st)
{
    auto wait = [&]
    {
        auto ret = WaitForSingleObject( timer_, INFINITE );
        return ret == WAIT_OBJECT_0;
    };

    while ( !st.stop_requested() )
    {
        if ( wait() )
        {
            StopWatchG::Wrap("wait end  ");
            auto lock = std::lock_guard< std::mutex >( mtx_ );
            if ( abort_ )
            {
                abort_ = false;
                task_ = nullptr;
            }
            else
            {
                if ( task_ )
                    task_();
                task_ = nullptr;
            }
            StopWatchG::Wrap("after task");
            StopWatchG::Print();
        }
    }
}

void PeriodWait(std::chrono::milliseconds ms)
{
    timeBeginPeriod(1);
    std::this_thread::sleep_for(ms);
    timeEndPeriod(1);
}