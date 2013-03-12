add_custom_command(
  OUTPUT cmd1
  COMMAND xcodebuild -project BonjourServer.xcodeproj -alltargets && cp -r build/Release/BonjourServer.app ${PROJECT_BIN_DIR}/mac
)

add_custom_target(BonjourServer.app DEPENDS cmd1)