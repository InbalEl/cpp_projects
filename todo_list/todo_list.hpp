#include <string>
#include <map>

class TodoList
{
public:
    TodoList(std::string list_name);
    ~TodoList() = default;

    void showList();
    void showTasksWithIds();
    int addItem(std::string new_task);
    void removeItem(int task_id);
    std::string getListName();
    void setListName(std::string name);

private:
    std::string m_list_name;
    std::map<int, std::string> m_task_list;
    static int m_task_id_counter;
};