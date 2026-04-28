@echo off
set UE_EDITOR="D:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJECT="H:\MahjongGame\MahjongGame.uproject"

echo Creating Lobby map...

rem Run the editor with Python script to create the map
"%UE_EDITOR%" "%PROJECT%" -Silent -NoSplash -ExecutePythonCommand="
import unreal
import os

content_dir = unreal.Paths.project_content_dir()
map_full_path = os.path.join(content_dir, 'Lobby', 'Maps', 'MahjongLobby.umap')
os.makedirs(os.path.dirname(map_full_path), exist_ok=True)

factory = unreal.WorldFactory()
new_world = factory.create_new_world(unreal.EWorldType.Game)

if new_world:
    level = new_world.get_current_level()

    sky_light_class = unreal.load_class(None, '/Script/Engine.SkyLight')
    if sky_light_class:
        transform = unreal.Transform()
        actor = new_world.actor_spawn(sky_light_class, transform)
        if actor:
            actor.set_actor_label('SkyLight')

    directional_light_class = unreal.load_class(None, '/Script/Engine.DirectionalLight')
    if directional_light_class:
        transform = unreal.Transform()
        actor = new_world.actor_spawn(directional_light_class, transform)
        if actor:
            actor.set_actor_label('Sun')

    player_start_class = unreal.load_class(None, '/Script/Engine.PlayerStart')
    if player_start_class:
        transform = unreal.Transform()
        actor = new_world.actor_spawn(player_start_class, transform)
        if actor:
            actor.set_actor_label('PlayerStart')

    unreal.EditorAssetLibrary.save_loaded_asset(new_world, True)
    unreal.log('Created: ' + map_full_path)
else:
    unreal.log_error('Failed to create world')
"

echo Done