//genexample
// This heavily-commented example introduces `raylib-openvr`'s core functionality in lieu of proper documentation.
// It may be a helpful starting point for creating a VR raylib project.

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

// ## Raylib Setup
int main() {
    InitWindow(800, 600, "Hello OpenVR");
    SetTargetFPS(120);

    // InitVr() starts OpenVR and reads information about the current device. If no SteamVR runtime is present or no
    // HMD is connected, InitVr() will log an error message and terminate the program.
    InitVr();

    // A VrRig is the main structure used for pose information. Its definition is as follows.
    // ```c
    //// typedef struct VrRig {
    ////     Vector3 trackingOrigin;         // Position of rig
    ////     VrTrackedDevice hmd;            // HMD relative to trackingOrigin
    ////     VrTrackedDevice controllers[2]; // Left and right controllers relative to trackingOrigin
    //// } VrRig;
    // ```

    // Each VrTrackedDevice (see OpenVR: TrackedDevicePose_t) stores device and tracking information.
    // ```c
    //// typedef struct VrTrackedDevice {
    ////    unsigned long id;    // OpenVR device ID
    ////    bool valid;          // Pose validity as reported by OpenVR
    ////    Matrix transform;    // Tracking origin -> device transform
    ////    Vector3 position;    // Device position relative to origin
    ////    Quaternion rotation; // Device rotation
    //// } VrTrackedDevice;
    // ```
    VrRig rig = { 0 };

    // ## Main Loop
    while (!WindowShouldClose()) {
        // UpdateVrTracking(&rig) must be called every frame. It reads VR input, and in doing so, focuses the
        // application so we can submit frames to the runtime. It updates the `hmd` and `controllers` fields of the
        // given `VrRig` with new information.
        UpdateVrTracking(&rig);

        BeginDrawing();
            // BeginVrDrawing() signals that we are rendering in stereo to both eyes of the HMD. When EndVrDrawing is
            // called, both frames will be sent to the runtime and rendered to the desktop window.
            BeginVrDrawing();
                // BeginMode3DVr() is an analog to BeginMode3D() that takes a VrRig instead of a Camera3D. It is
                // finalized with EndMode3D().
                BeginMode3DVr(rig);
                    ClearBackground(RAYWHITE);
                    DrawGrid(10, 5);

                    // This example draws gizmos at the left and right controller positions. It uses RLGL functions to
                    // correctly position and orient them.
                    rlPushMatrix();
                    rlScalef(0.25f, 0.25f, 0.25f);
                    rlPushMatrix();
                    rlMultMatrixf(MatrixToFloat(rig.controllers[0].transform));
                    DrawGizmo(Vector3Zero());
                    rlPopMatrix();
                    rlMultMatrixf(MatrixToFloat(rig.controllers[1].transform));
                    DrawGizmo(Vector3Zero());
                    rlPopMatrix();
                EndMode3D();
            EndVrDrawing();

            // After rendering to the headset, we can continue drawing to the desktop window.
            DrawFPS(2, 2);
        EndDrawing();
    } // main loop

    // ## Cleanup
    // CloseVr() gracefully shuts down OpenVR and frees resources.
    CloseVr();
    CloseWindow();

    return 0;
} // main