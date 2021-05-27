#include <string>
#include <iostream>
#include <map>
#include "todo_list.hpp"

int TodoList::m_task_id_counter = 0;

/******************************************************************************/
TodoList::TodoList(std::string list_name) : m_list_name(list_name)
{}

/*----------------------------------------------------------------------------*/
void TodoList::showList()
{
    for (auto& entry : m_task_list)
    {
        std::cout << entry.second << std::endl;
    }
}

/*----------------------------------------------------------------------------*/
void TodoList::showTasksWithIds()
{
    for (auto& entry : m_task_list)
    {
        std::cout << entry.first << "\t" << entry.second << std::endl;
    }
}

/*----------------------------------------------------------------------------*/
int TodoList::addItem(std::string new_task)
{
    m_task_list[m_task_id_counter] = new_task;
    return (m_task_id_counter++);
}

/*----------------------------------------------------------------------------*/
void TodoList::removeItem(int task_id)
{
    m_task_list.erase(task_id);
}

/*----------------------------------------------------------------------------*/
std::string TodoList::getListName()
{
    return m_list_name;
}

/*----------------------------------------------------------------------------*/
void TodoList::setListName(std::string name)
{
    m_list_name = name;
}

/******************************************************************************/