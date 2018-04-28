#include "work_item.hpp"

WorkItem::WorkItem(int type) {
  std::cout << "New Work Item" << std::endl;
  switch(type) {
    case 0:
        this->m_action = "Volume change";
        this->m_priority = 0;
        break;
    case 1:
        this->m_action = "EQ change";
        this->m_priority = 1;
        break;
    case 2:
        this->m_action = "Disable track";
        this->m_priority = 2;
        break;
  }
}

  WorkItem::~WorkItem() { std::cout << "deleted WorkItem" << std::endl;}

  std::string WorkItem::getAction() { return this->m_action;}

  std::ostream& operator<<(std::ostream &out, const WorkItem &w) {
    out << "Action: " << w.m_action << ", with priority: " << w.m_priority;
    return out;
  }
