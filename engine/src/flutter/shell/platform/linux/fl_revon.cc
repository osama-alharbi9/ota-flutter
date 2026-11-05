#include "flutter/shell/platform/linux/fl_revon.h"

#include <fstream>
#include <sstream>
#include <string>

#include "flutter/fml/file.h"
#include "flutter/fml/logging.h"
#include "flutter/fml/paths.h"
#include "flutter/shell/common/revon/revon.h"
#include "rapidjson/document.h"
#include "third_party/tonic/filesystem/filesystem/file.h"

// Namespaced to avoid Google style warnings.
namespace flutter {

gboolean SetUpRevon(const char* assets_path, std::string& patch_path) {
  auto revon_yaml_path =
      fml::paths::JoinPaths({assets_path, "revon.yaml"});
  std::string revon_yaml_contents("");
  if (!filesystem::ReadFileToString(revon_yaml_path,
                                    &revon_yaml_contents)) {
    FML_LOG(ERROR) << "Failed to read revon.yaml.";
    return false;
  }

  std::string code_cache_path =
      fml::paths::JoinPaths({g_get_home_dir(), ".revon_cache"});
  auto executable_location = fml::paths::GetExecutableDirectoryPath().second;
  auto app_path =
      fml::paths::JoinPaths({executable_location, "lib", "libapp.so"});
  auto version_json_path = fml::paths::JoinPaths({assets_path, "version.json"});
  std::ifstream input(version_json_path);
  if (!input) {
    return false;
  }
  std::string json_contents{std::istreambuf_iterator<char>(input),
                            std::istreambuf_iterator<char>()};

  rapidjson::Document json_doc;
  json_doc.Parse(json_contents.c_str());
  if (json_doc.HasParseError()) {
    // Could not parse version file, aborting.
    return false;
  }

  const auto version_map = json_doc.GetObject();
  ReleaseVersion release_version{version_map["version"].GetString(),
                                 version_map["build_number"].GetString()};

  RevonConfigArgs revon_args(code_cache_path, code_cache_path, app_path,
                                     revon_yaml_contents, release_version);
  return ConfigureRevon(revon_args, patch_path);
}

}  // namespace flutter
