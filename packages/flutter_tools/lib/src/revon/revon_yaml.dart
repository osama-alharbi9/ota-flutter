// Copyright 2024 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'package:yaml/yaml.dart';
import 'package:yaml_edit/yaml_edit.dart';

import '../base/file_system.dart';
import '../globals.dart' as globals;

void updateRevonYaml(
  String? flavor,
  String revonYamlPath, {
  required Map<String, String> environment,
}) {
  final File revonYaml = globals.fs.file(revonYamlPath);
  if (!revonYaml.existsSync()) {
    throw Exception('revon.yaml not found at $revonYamlPath');
  }
  final YamlDocument input = loadYamlDocument(revonYaml.readAsStringSync());
  final YamlMap yamlMap = input.contents as YamlMap;
  final Map<String, dynamic> compiled = compileRevonYaml(
    yamlMap,
    flavor: flavor,
    environment: environment,
  );
  // Currently we write out over the same yaml file, we should fix this to
  // write to a new .json file instead and avoid naming confusion between the
  // input and compiled files.
  final YamlEditor yamlEditor = YamlEditor('');
  yamlEditor.update(<Object?>[], compiled);
  revonYaml.writeAsStringSync(yamlEditor.toString(), flush: true);
}

String appIdForFlavor(YamlMap yamlMap, {required String? flavor}) {
  if (flavor == null || flavor.isEmpty) {
    final String? defaultAppId = yamlMap['app_id'] as String?;
    if (defaultAppId == null || defaultAppId.isEmpty) {
      throw Exception('Cannot find "app_id" in revon.yaml');
    }
    return defaultAppId;
  }

  final YamlMap? yamlFlavors = yamlMap['flavors'] as YamlMap?;
  if (yamlFlavors == null) {
    throw Exception('Cannot find "flavors" in revon.yaml.');
  }
  final String? flavorAppId = yamlFlavors[flavor] as String?;
  if (flavorAppId == null || flavorAppId.isEmpty) {
    throw Exception('Cannot find "app_id" for $flavor in revon.yaml');
  }
  return flavorAppId;
}

Map<String, dynamic> compileRevonYaml(
  YamlMap yamlMap, {
  required String? flavor,
  required Map<String, String> environment,
}) {
  final String appId = appIdForFlavor(yamlMap, flavor: flavor);
  final Map<String, dynamic> compiled = <String, dynamic>{'app_id': appId};
  void copyIfSet(String key) {
    if (yamlMap[key] != null) {
      compiled[key] = yamlMap[key];
    }
  }

  copyIfSet('base_url');
  copyIfSet('auto_update');
  copyIfSet('patch_verification');
  final String? revonPublicKeyEnvVar = environment['REVON_PUBLIC_KEY'];
  if (revonPublicKeyEnvVar != null) {
    compiled['patch_public_key'] = revonPublicKeyEnvVar;
  }
  return compiled;
}
