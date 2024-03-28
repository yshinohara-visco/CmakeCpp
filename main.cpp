#include <iostream>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <thread>

//どこかでAddしている最中にdeleteされると例外が発生する?
class MyClass1
{
public:
	void Add() {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_hoge++;
	}

private:
	std::mutex m_mutex;
	int m_hoge = 0;
};

//Delete()で例外が発生する
class MyClass2
{
public:
	void Init(int id) {
        m_mapStruct[id];
    }

	bool Add(int id) {
		std::lock_guard<std::mutex> lock(m_mapStruct[id].mutex);
		if (m_mapStruct.count(id)) {
            m_mapStruct[id].hoge++;
			return true;
		}
		else {
            std::cout << "not found" << std::endl;
			return false;
		}
	}

	void Delete(int id) {
		std::lock_guard<std::mutex> lock(m_mapStruct[id].mutex);
		m_mapStruct.erase(id); //->lock中のmutexの解放で例外発生
	}

private:
	struct MyStruct
	{
		std::mutex mutex;
		int hoge = 0;
	};
	std::unordered_map<int, MyStruct> m_mapStruct;
};

//mutexの解放をどうするか
class MyClass3
{
public:
	void Add(int id) {
        std::lock_guard<std::mutex> lock(m_mapMutex[id]);
        m_mapHoge[id]++;
    }

	void Delete(int id) {
        std::lock_guard<std::mutex> lock(m_mapMutex[id]);
        m_mapHoge.erase(id);
		//m_mapHoge.erase(id); 例外が起きる mutexの解法はいつすべきか
    }

private:
	std::unordered_map<int, std::mutex> m_mapMutex;
	std::unordered_map<int, int> m_mapHoge;
};

class MyClass4
{
public:
	void Init(int id) {
        m_mapStruct.insert(std::make_pair(id, std::make_shared<MyStruct>()));
    }

	bool Add(int id) {
        auto sp = GetSp(id);
		if (sp) {
			std::lock_guard<std::mutex> lock(sp->mutex);
			std::cout << "Add[" << id << "] " << sp->hoge << "->" << sp->hoge + 1 << std::endl;
			sp->hoge++;
			return true;
		}
		else {
			return false;
		}
    }

	void Delete(int id) {
        DeleteSp(id);
		std::cout << "Delete[" << id << "]" << std::endl;
	}

private:
	struct MyStruct
	{
		std::mutex mutex;
		int hoge = 0;
	};
	using spStruct = std::shared_ptr<MyStruct>;
	std::unordered_map<int, spStruct> m_mapStruct;
	std::mutex	m_mutexMap;

	spStruct GetSp(int id) {
        std::lock_guard<std::mutex> lock(m_mutexMap);
		if (m_mapStruct.count(id)) {
            return m_mapStruct[id];
		}
		else {
			return nullptr;
		}
    }

	void DeleteSp(int id) {
        std::lock_guard<std::mutex> lock(m_mutexMap);
        m_mapStruct.erase(id);
    }
};

int main()
{
	//std::cout << "MyClass" << std::endl;
	//auto myClass = new MyClass1();
	//myClass->Add();
	//delete myClass; //他スレッドでAdd中なら例外になる?

	//std::cout << "MyClass2" << std::endl;
	//auto myClass2 = MyClass2();
	//myClass2.Add(1);
	//myClass2.Delete(1); //->例外

	auto myClass4 = MyClass4();
	myClass4.Init(1);
	myClass4.Init(2);
	auto th1 = std::thread([&myClass4] {
		for (size_t i = 0; i < 100; i++)
		{
			myClass4.Add(1);
			myClass4.Add(1);
			myClass4.Add(2);
		}
		});
	auto th2 = std::thread([&myClass4] {
		for (size_t i = 0; i < 100; i++)
		{
            myClass4.Add(1);
			myClass4.Add(2);
			myClass4.Add(2);
        }
        });
	auto th3 = std::thread([&myClass4] {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		myClass4.Delete(1);
		myClass4.Delete(2);
		});
	th1.join();
	th2.join();
	th3.join();

	return 0;
}
