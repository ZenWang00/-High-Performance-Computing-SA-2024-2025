import os
import time
import random
from datetime import datetime, timedelta

def random_time_between(start_date, end_date):
    """
    生成指定日期范围内的随机时间戳
    :param start_date: 开始日期 (datetime 对象)
    :param end_date: 结束日期 (datetime 对象)
    :return: 随机的 UNIX 时间戳
    """
    delta = end_date - start_date
    random_seconds = random.randint(0, int(delta.total_seconds()))
    return (start_date + timedelta(seconds=random_seconds)).timestamp()

def update_file_times(directory, start_date, end_date):
    """
    递归修改指定目录下所有文件的修改时间 (mtime) 和访问时间 (atime)。
    修改时间随机设置在指定日期范围内。
    :param directory: 要修改的文件夹路径
    :param start_date: 开始日期 (datetime 对象)
    :param end_date: 结束日期 (datetime 对象)
    """
    for root, _, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            try:
                # 随机生成新的修改时间
                new_time = random_time_between(start_date, end_date)
                # 修改文件的访问时间和修改时间
                os.utime(file_path, (new_time, new_time))
                print(f"Updated: {file_path} -> {datetime.fromtimestamp(new_time)}")
            except Exception as e:
                print(f"Error updating {file_path}: {e}")

if __name__ == "__main__":
    folder_path = input("请输入文件夹路径: ").strip()
    # 指定时间范围
    start_date = datetime(2024, 12, 10)  # 开始时间：2024年12月10日
    end_date = datetime(2024, 12, 17)    # 结束时间：2024年12月17日

    update_file_times(folder_path, start_date, end_date)
