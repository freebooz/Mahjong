# Unreal Engine Python Script - Create All Mahjong Blueprints
# Run in UE Editor: Window -> Developer Tools -> Python Editor

try:
    import unreal
except ImportError:
    raise ImportError(
        "This script must be run inside Unreal Engine Editor.\n"
        "To execute:\n"
        "  1. Open UE Editor\n"
        "  2. Window -> Developer Tools -> Python Editor\n"
        "  3. Open this script and run"
    )

def log_info(msg):
    unreal.log("[INFO] {}".format(msg))

def log_success(msg):
    unreal.log("[SUCCESS] {}".format(msg))

def log_error(msg):
    unreal.log_error("[ERROR] {}".format(msg))

def log_warning(msg):
    unreal.log_warning("[WARNING] {}".format(msg))

def create_blueprint(parent_class, bp_name, package_path):
    """Create a Blueprint that inherits from a C++ class"""
    if not parent_class:
        log_error("Cannot create {} - parent class is None".format(bp_name))
        return False

    # Check if Blueprint already exists
    existing_bp_path = "{}/{}".format(package_path, bp_name)
    if unreal.EditorAssetLibrary.does_asset_exist(existing_bp_path):
        log_warning("Blueprint {} already exists, skipping...".format(bp_name))
        return True

    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.BlueprintFactory()
    factory.set_editor_property('parent_class', parent_class)

    bp_asset = asset_tools.create_asset(
        asset_name=bp_name,
        package_path=package_path,
        asset_class=unreal.Blueprint,
        factory=factory
    )

    if bp_asset:
        log_success("Created {}".format(bp_name))
        return True
    else:
        log_error("Failed to create {}".format(bp_name))
        return False

def main():
    log_info("=== Creating Mahjong Game Blueprint Classes ===")

    content_path = "/Game/UI/Blueprints"

    # All C++ base classes to create Blueprints for
    bp_classes = [
        # UI Widget classes (UUserWidget-based)
        ("UGameWidgetBase", "BP_GameWidget"),
        ("USettingsWidgetBase", "BP_SettingsWidget"),
        ("UHUDWidgetBase", "BP_HUDWidget"),
        ("ULoginWidgetBase", "BP_LoginWidget"),
        ("ULobbyWidgetBase", "BP_LobbyWidget"),
        ("URoomWidgetBase", "BP_RoomWidget"),
    ]

    # First, verify all parent classes exist
    log_info("Verifying parent classes...")
    missing_classes = []
    found_classes = []
    for parent_class_name, bp_name in bp_classes:
        parent_class_path = "/Script/MahjongGame.{}".format(parent_class_name)
        parent_class = unreal.load_class(None, parent_class_path)
        if parent_class:
            log_success("Found: {}".format(parent_class_name))
            found_classes.append((parent_class, bp_name))
        else:
            log_error("Missing: {}".format(parent_class_name))
            missing_classes.append(parent_class_name)

    if missing_classes:
        log_error("Missing {} parent class(s). Please compile the C++ code first.".format(len(missing_classes)))
        log_error("Missing: {}".format(", ".join(missing_classes)))
        return

    # Create folder if it doesn't exist
    log_info("Creating content folder: {}".format(content_path))
    if not unreal.EditorAssetLibrary.does_directory_exist(content_path):
        unreal.EditorAssetLibrary.make_directory(content_path)
        log_success("Created folder: {}".format(content_path))
    else:
        log_info("Folder already exists: {}".format(content_path))

    # Create all blueprints
    log_info("Creating Blueprints...")
    success_count = 0
    fail_count = 0

    for parent_class, bp_name in found_classes:
        if create_blueprint(parent_class, bp_name, content_path):
            success_count += 1
        else:
            fail_count += 1

    # Summary
    log_info("=== Blueprint Creation Complete ===")
    log_info("Created: {} Blueprints".format(success_count))
    if fail_count > 0:
        log_error("Failed: {} Blueprints".format(fail_count))
    else:
        log_success("All Blueprints created successfully!")

    log_info("")
    log_info("Next steps:")
    log_info("1. Open each Blueprint in the Editor")
    log_info("2. For UI Widgets: Bind the widget components in the Designer")
    log_info("3. Compile and save each Blueprint")

if __name__ == "__main__":
    main()