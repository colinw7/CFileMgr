#ifndef CDirScope_H
#define CDirScope_H

#include <CDir.h>

struct CDirScope {
  const CDir *dir;
  bool        entered;

  explicit CDirScope(const std::string &dirname) :
   dir(0) {
    entered = CDir::enter(dirname);
  }

  explicit CDirScope(const CDir &dir1) :
   dir(&dir1) {
    entered = dir->enter();
  }

 ~CDirScope() {
    if (entered) {
      if (dir)
        dir->leave();
      else
        CDir::leave();
    }
  }

  bool valid() const { return entered; }
};

#endif
