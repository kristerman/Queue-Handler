#ifndef __WORK_ITEM_HPP
#define __WORK_ITEM_HPP
#include <iostream>
#include <string>

class WorkItem{
public:
  WorkItem(int type=0);

  friend std::ostream& operator<<(std::ostream &out, const WorkItem &w);

private:
  std::string m_action;
  int m_priority;
};

#endif
