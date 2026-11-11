# No Ship Cube (Geode mod)

Hides the cube that rides inside the ship, UFO and jetpack. Built for Geode 5.9 / GD 2.2081.

## Building it (no PC needed)

GitHub builds it for you, for every platform (Android, Windows, Mac, iOS).

1. Make a free GitHub account and create a new **empty** repository (e.g. `no-ship-cube`).
2. Upload every file from this zip. The `.github/workflows/multi-platform.yml` file must keep that exact folder path.
   (On the GitHub website: "Add file" → "Upload files" and drag the whole unzipped folder in.)
3. Open the repo's **Actions** tab. The "Build Geode Mod" run starts automatically after you upload. If it doesn't, click it and press "Run workflow".
4. When it finishes (green check, about 5–10 min), open the run and download **Build Output** at the bottom.
5. Unzip it. You get `gdrynk.no-ship-cube.geode`.

## Installing on Android

Copy `gdrynk.no-ship-cube.geode` into Geode's mods folder:

`Android/media/com.geode.launcher/game/geode/mods/`

Restart GD. The mod shows up as **No Ship Cube** in the Geode mod list, where its settings are.

## Settings

- Hide in Ship
- Hide in UFO
- Hide in Jetpack

All on by default.
