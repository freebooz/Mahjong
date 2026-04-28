# Unreal Engine Python Script - Create Lobby Map
# Run in UE Editor: Window -> Developer Tools -> Python Editor

try:
    import unreal
except ImportError:
    raise ImportError(
        "This script must be run inside Unreal Engine Editor.\n"
        "Run via: Window -> Developer Tools -> Python Editor"
    )

def log_info(msg):
    unreal.log("[INFO] {}".format(msg))

def log_success(msg):
    unreal.log("[SUCCESS] {}".format(msg))

def log_error(msg):
    unreal.log_error("[ERROR] {}".format(msg))

def main():
    log_info("=== Creating Lobby Map ===")

    # Map path
    map_path = "/Game/Lobby/Maps/MahjongLobby"
    full_map_path = map_path + ".umap"

    # Check if map already exists
    if unreal.EditorAssetLibrary.does_asset_exist(full_map_path):
        log_success("Map already exists: {}".format(full_map_path))
        return

    # Create directory
    dir_path = "/Game/Lobby/Maps"
    if not unreal.EditorAssetLibrary.does_directory_exist(dir_path):
        unreal.EditorAssetLibrary.make_directory(dir_path)
        log_info("Created directory: {}".format(dir_path))

    # Create new map using EditorLoadingAndSavingUtils.new_level
    # This is the correct UE API for creating new maps
    unreal.EditorLoadingAndSavingUtils.new_level(full_map_path)

    log_success("Created map: {}".format(full_map_path))
    log_info("=== Done ===")

if __name__ == "__main__":
    main()