
#ifndef __DEMONEYE__
#define __DEMONEYE__ 1



/* Header Inclusion Tree:
.   - '+' = branch between or branch into multiple inheritance lines.
.   - '<' and '>' stipulate an exclusive direction of flow over an inheritance line.
.   - If two lines intersect without a '+' joining, them that they do not connect,
.     they are perpendicular inheritance lines that cross over one another.
.   - If the name of a file or directory is lies directly over top, underneath, or
.     next to a '+' symbol without lines connecting them, the file or directory is
.     not part of the junction, file names and directory names can only connect to
.     lines, not '+' sign junctions.
.   - Files at the top are dependencies for files that are lower down.
.   - Files within a directory will be listed in their own inclusion tree underneath
.     the directory name.
.   - If a directory is listed as having a dependency, it is a common dependency of
.     all files within that directory.
.   - When a header leaves inheritance to a directory by the same name, the files in
.     that directory are submodules, and will be loaded by that header.
.   - Inheritance flows down and to the right, only left if there is a downward line
.     of inheritance (or multiple) to the left, inheritance can flow left and right
.     at the same time, but always seeks downward.
.
.  setup.hpp----setup/
.   |             config.hpp
.   |              |
.   |             types.hpp
.   |              |
.   |             errno.hpp
.   |              |
.   +--------------+
.   |
.  memory.hpp-------iter.hpp
.   |                |
.  threading.hpp     +----------+---------+---------------+---------+
.   |                |          |         |               |         |
.  io.hpp           string.hpp array.hpp linked_list.hpp table.hpp math.hpp
.   |                |          |   serialize.hpp  |      |         |
.   |                |          |    |      |      |      |         |
.   +----------------+  +------------+      |      |      |         |
.   |                |  |       |           |      |      |         |
.   |                |  |       +---+       |      |      |         |
.   |               serialize/      |       |      |      |         |
.   |                  general.hpp--+-------+------+------+         |
.   |                   |           |       |      |      |         |
.   |                   |          array.hpp|llist.hpp table.hpp    |
.   |                   |           |       | |         |           |
.   |                   |           |       | |         |    +-----<+
.   +-------------------+-----------+---------+---------+    |
.   |                                       |                |
.   |                                       +----------------+
.  resmgr.hpp----resmgr/                                     |
.   |              reslocs.hpp  restypes.hpp             mathio.hpp
.  logger.hpp                                             |
.   |      |                                              |
.  +|-----------------------------------------------------+
.  ||      |
.  ||      |
.  gl.hpp periphio.hpp----periphio/
.   |      |                kbd.hpp  mouse.hpp  audio.hpp xinput.hpp
.   +------+
.   |
.  env.hpp
.*/
#include "demoneye/env.hpp"



#endif // __DEMONEYE__
