# GoogleTestヘルパー関数を使用してテストターゲットを作成
create_gtest_target(
  math_operations_test
  "test_math_operations.cpp"
  "math_operations"
)

# AsyncDataライブラリのテスト
create_gtest_target(
  async_data_test
  "test_async_data.cpp"
  "async_data"
)

# AsyncDataAsync（非同期版）のテスト
create_gtest_target(
  async_data_async_test
  "test_async_data_async.cpp"
  "async_data"
)