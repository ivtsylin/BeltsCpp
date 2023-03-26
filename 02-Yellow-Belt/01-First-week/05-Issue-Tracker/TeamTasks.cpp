#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <tuple>

using namespace std;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return data.at(person);
    }
    
    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++data[person][TaskStatus::NEW];
    }
    // Обновить статусы по данному количеству задач конкретного разработчика,
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        const TasksInfo &init = data[person];
        TasksInfo result;
        TasksInfo changed;
        TasksInfo unchanged;
        
        for (TaskStatus status = TaskStatus::NEW; status != TaskStatus::DONE; status = next_status(status)) {
            change_status (init, status, task_count, changed, unchanged, result);
        }
        if (init.count(TaskStatus::DONE)) {
            result[TaskStatus::DONE] += init.at(TaskStatus::DONE);
        }
        
        data[person] = result;
        
        return make_tuple(changed, unchanged);
    }
private:
    map<string, TasksInfo> data;
    inline TaskStatus next_status(TaskStatus cur) {
        if (cur == TaskStatus::NEW) {
            return TaskStatus::IN_PROGRESS;
        } else if (cur == TaskStatus::IN_PROGRESS) {
            return TaskStatus::TESTING;
        }
        return TaskStatus::DONE;
    };
    // Предполагаем, что функция запускается только для 3 первых статусов
    inline void change_status (const TasksInfo &src, const TaskStatus &status, int &task_count, TasksInfo &changed, TasksInfo &unchanged, TasksInfo &result) {
        if (src.count(status)) {
            if (!task_count) {
                unchanged[status] = src.at(status);
                result[status] += src.at(status);
                return;
            } else {
                int cur_count = min (task_count, src.at(status));
                task_count -= cur_count;
                changed[next_status(status)] = cur_count;
                result[next_status(status)] = cur_count;
                if (src.at(status) > cur_count) {
                    unchanged[status] = src.at(status) - cur_count;
                    result[status] += src.at(status) - cur_count;
                }
            }
        }
    }
};
