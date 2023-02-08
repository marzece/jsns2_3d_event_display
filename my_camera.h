#include <math.h>
#define CAMERA_FREE_MOUSE_SENSITIVITY                   0.01f
#define CAMERA_FREE_DISTANCE_MIN_CLAMP                  0.3f
#define CAMERA_FREE_DISTANCE_MAX_CLAMP                  120.0f
#define CAMERA_FREE_MIN_CLAMP                           85.0f
#define CAMERA_FREE_MAX_CLAMP                          -85.0f
#define CAMERA_FREE_SMOOTH_ZOOM_SENSITIVITY             0.05f
#define CAMERA_FREE_PANNING_DIVIDER                     5.1f

// Camera mouse movement sensitivity
#define CAMERA_MOUSE_MOVE_SENSITIVITY                   0.5f    // TODO: it should be independant of framerate
#define CAMERA_MOUSE_SCROLL_SENSITIVITY                 0.1f

#define CAMERA_THIRD_PERSON_DISTANCE_CLAMP              1.2f
#define CAMERA_THIRD_PERSON_MIN_CLAMP                   5.0f
#define CAMERA_THIRD_PERSON_MAX_CLAMP                  -85.0f


typedef struct {
    unsigned int mode;              // Current camera mode
    float targetDistance;           // Camera distance from position to target
    float playerEyesPosition;       // Player eyes position from ground (in meters)
    Vector2 angle;                  // Camera angle in plane XZ

    // Camera movement control keys
    int smoothZoomControl;          // Smooth zoom control key
    int altControl;                 // Alternative control key
    int panControl;                 // Pan view control key
} CameraData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

/*
typedef struct Camera3D {
    Vector3 position;       // Camera position
    Vector3 target;         // Camera target it looks-at
    Vector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} Camera3D;
*/
#define SQR(x) ((x)*(x))
// Calculate two vectors cross product
Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2) {
    return (Vector3){ v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
}

static float MAG2(Vector3 v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

static Vector3 VecAdd(Vector3 v1, Vector3 v2, float s) {
    return (Vector3) {v1.x+s*v2.x, v1.y+s*v2.y, v1.z+s*v2.z};
}

static Vector3 VecScale(Vector3 v, float s) {
    return (Vector3) {v.x*s, v.y*s, v.z*s};
}

static void MyUpdateCamera(Camera3D* cam) {

    // Mouse movement detection
    Vector2 mousePositionDelta = GetMouseDelta();
    float mouseWheelMove = -GetMouseWheelMove();

    bool shift_key_down = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    if(shift_key_down) {
        // Rotate around the target position with the camera up direction as
        // the axis of rotation
        Vector3 dir = Vector3CrossProduct(cam->up, cam->position);
        dir = VecScale(dir, 1.0/sqrtf(MAG2(dir))); // Normalize the direction

        float prev_mag = MAG2(cam->position);
        cam->position = VecAdd(cam->position, dir, 10.0*mouseWheelMove*CAMERA_MOUSE_SCROLL_SENSITIVITY);
        float new_mag = MAG2(cam->position);

        // I think there's a floating point issue in the cross-product, the
        // result isn't PERFECTLY perpendicular to the two input vector,
        // causing the camaera to float away from the origin instead of just
        // rotating around it.  So this just pulls the camera in a bit instead
        // of letting it float away
        if(new_mag != prev_mag) {
            float factor = sqrtf(prev_mag/new_mag);
            cam->position.x = cam->position.x*factor;
            cam->position.y = cam->position.y*factor;
            cam->position.z = cam->position.z*factor;
        }

    } else {
        // Move closer or further from zero
        cam->position = VecAdd(cam->position, cam->position, mouseWheelMove*CAMERA_MOUSE_SCROLL_SENSITIVITY);
    }
    // Camera orientation calculation
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector3 dir1 = cam->up;
        Vector3 dir2 = Vector3CrossProduct(cam->up, cam->position);
        // Normalize the direction vectors
        dir2 = VecScale(dir2, sqrtf(MAG2(dir2)));
        dir1 = VecScale(dir1, sqrtf(MAG2(dir1)));

        float prev_mag = MAG2(cam->position);
        cam->position = VecAdd(cam->position, dir1, -1.0*mousePositionDelta.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);
        cam->position = VecAdd(cam->position, dir2, 0.0001*mousePositionDelta.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
        float new_mag = MAG2(cam->position);

        if(new_mag != prev_mag) {
            float factor = sqrtf(prev_mag/new_mag);
            cam->position = VecScale(cam->position, factor);
        }
        //cam->target.x += (mousePositionDelta.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
        //cam->target.z += (mousePositionDelta.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);
    }


}
