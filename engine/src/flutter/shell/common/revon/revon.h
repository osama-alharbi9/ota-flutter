#ifndef FLUTTER_SHELL_COMMON_REVON_REVON_H_
#define FLUTTER_SHELL_COMMON_REVON_REVON_H_

#include "flutter/common/settings.h"
#include "flutter/fml/memory/ref_ptr.h"
#include "shell/platform/embedder/embedder.h"

namespace flutter {

class DartSnapshot;

/// Version and build number of the release.
/// Used by RevonConfigArgs.
struct ReleaseVersion {
  std::string version;
  std::string build_number;
};

/// Arguments for ConfigureRevon.
/// Used by Desktop implementations.
struct RevonConfigArgs {
  std::string code_cache_path;
  std::string app_storage_path;
  std::string release_app_library_path;
  std::string revon_yaml;
  ReleaseVersion release_version;

  RevonConfigArgs(std::string code_cache_path,
                      std::string app_storage_path,
                      std::string release_app_library_path,
                      std::string revon_yaml,
                      ReleaseVersion release_version)
      : code_cache_path(code_cache_path),
        app_storage_path(app_storage_path),
        release_app_library_path(release_app_library_path),
        revon_yaml(revon_yaml),
        release_version(release_version) {}
};

/// Newer api, used by Desktop implementations.
/// Does not directly manipulate Settings.
bool ConfigureRevon(const RevonConfigArgs& args,
                        std::string& patch_path);

/// Older api used by iOS and Android, directly manipulates Settings.
void ConfigureRevon(std::string code_cache_path,
                        std::string app_storage_path,
                        Settings& settings,
                        const std::string& revon_yaml,
                        const std::string& version,
                        const std::string& version_code);

/// Used for reading app_id from revon.yaml.
/// Exposed for testing.
std::string GetValueFromYaml(const std::string& yaml, const std::string& key);

}  // namespace flutter

#endif  // FLUTTER_SHELL_COMMON_REVON_REVON_H_
