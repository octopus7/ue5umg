# Model: GPT-6.1 Sol; reasoning: Medium
import unreal
assert unreal.GenerateSolPanelAssets.generate_widgets(), "Sol panel WBP generation failed"
for name in ("Main", "Popup"):
    asset = unreal.load_asset("/Game/UI/Sol/WBP_" + name + "_GPT61Sol")
    assert asset is not None
    unreal.BlueprintEditorLibrary.compile_blueprint(asset)
    unreal.EditorAssetLibrary.save_loaded_asset(asset)
unreal.log("UMG57_SOL_PANEL_SETUP_COMPLETE")
