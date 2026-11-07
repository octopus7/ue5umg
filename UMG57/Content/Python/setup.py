import unreal
assert unreal.GenerateTestAssets.generate_widgets()
level=unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
level.new_level('/Game/Maps/UMGTest')
actors=unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
for path,position,scale in [('/Engine/BasicShapes/Plane',unreal.Vector(0,0,0),unreal.Vector(10,10,1)),('/Engine/BasicShapes/Cube',unreal.Vector(0,0,50),unreal.Vector(1,1,1))]:
    a=actors.spawn_actor_from_class(unreal.StaticMeshActor,position)
    a.static_mesh_component.set_static_mesh(unreal.load_asset(path))
    a.set_actor_scale3d(scale)
actors.spawn_actor_from_class(unreal.PlayerStart,unreal.Vector(-500,0,150))
actors.spawn_actor_from_class(unreal.DirectionalLight,unreal.Vector(0,0,500))
actors.spawn_actor_from_class(unreal.SkyLight,unreal.Vector(0,0,500))
assert level.save_current_level()
unreal.log('UMG57_SETUP_COMPLETE')
