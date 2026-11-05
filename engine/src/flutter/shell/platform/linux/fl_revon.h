#ifndef FLUTTER_SHELL_PLATFORM_LINUX_FL_REVON_H_
#define FLUTTER_SHELL_PLATFORM_LINUX_FL_REVON_H_

#include <glib-object.h>
#include <string>

namespace flutter {

gboolean SetUpRevon(const char* assets_path, std::string& patch_path);

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_LINUX_FL_REVON_H_
