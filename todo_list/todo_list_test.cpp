#include <iostream>
#include <vector>
#include "todo_list.hpp"

class TodoListTester
{
public:
    TodoListTester(TodoList& todo_list);
    ~TodoListTester() = default;

    void testInsersion();
    void testGetListName();
    void testShowList();
    void testShowTasksWithIds();
    void testsetListName();    
    void testClearList();    
    void testRemoval();

private:
    TodoList& m_todo_list;
    std::vector<int> m_tasks_ids;
    std::vector<std::string> m_inserted_items;
};

TodoListTester::TodoListTester(TodoList& todo_list) : m_todo_list(todo_list)
{}

void TodoListTester::testInsersion()
{  
    std::cout << "--testInsersion()--\n" << std::endl;
    // TODO: this test can be better
    m_tasks_ids.push_back(m_todo_list.addItem("Clean toilet"));
    m_inserted_items.push_back("Clean toilet");
    m_tasks_ids.push_back(m_todo_list.addItem("Cook ravioli"));
    m_inserted_items.push_back("Cook ravioli");
    m_tasks_ids.push_back(m_todo_list.addItem("Build a todo list"));
    m_inserted_items.push_back("Build a todo list");
}

void TodoListTester::testGetListName()
{
    std::cout << "--testGetListName()--\n" << std::endl;
    std::cout << m_todo_list.getListName() << std:: endl;
}

void TodoListTester::testShowList()
{
    std::cout << "--testShowList()--\n" << std::endl;
    std::cout << "    >> items added in test <<" << std::endl;
    for (auto& task : m_inserted_items)
    {
        std::cout << task << std::endl;
    }

    std::cout << "    >> showList() <<" << std::endl;
    m_todo_list.showList();
}

void TodoListTester::testShowTasksWithIds()
{
    std::cout << "--testShowTasksWithIds()--\n" << std::endl;
    // TODO: can be better
    m_todo_list.showTasksWithIds();
}

void TodoListTester::testsetListName()
{
    std::cout << "--testsetListName()--\n" << std::endl;
    std::cout << m_todo_list.getListName() << std::endl;
    std::cout << "    >> Changing name to 'New List Name' <<" << std::endl;
    m_todo_list.setListName("New List Name");
    std::cout << m_todo_list.getListName() << std::endl;   
}

void TodoListTester::testRemoval()
{
    std::cout << "--testRemoval()--\n" << std::endl;
    m_todo_list.showTasksWithIds();
    int task_id = m_tasks_ids.back();
    m_tasks_ids.pop_back();
    std::cout << "    >> removing task << " << task_id << std::endl;
    m_todo_list.showTasksWithIds();
}

void TodoListTester::testClearList()
{
    std::cout << "--testClearList()--\n" << std::endl;
    while (!m_inserted_items.empty())
    {
        m_todo_list.removeItem(m_tasks_ids.back());
        m_tasks_ids.pop_back();
    }

    m_todo_list.removeItem(4);
}

/******************************************************************************/

int main()
{
    TodoList list("MyList");
    TodoListTester new_list_tester(list);

    new_list_tester.testGetListName();
    new_list_tester.testInsersion();
    new_list_tester.testShowList();
    new_list_tester.testRemoval();
    new_list_tester.testShowTasksWithIds();
    new_list_tester.testsetListName();    
    // new_list_tester.testClearList();

    return (EXIT_SUCCESS);
}