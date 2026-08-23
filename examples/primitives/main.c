#include <prgl/camera.h>
#include <prgl/game.h>
#include <prgl/game_object.h>
#include <prgl/input.h>
#include <prgl/mesh.h>
#include <prgl/render.h>
#include <prgl/screen.h>
#include <prgl/texture.h>
#include <prgl/types.h>

#include <stdio.h>

struct PRGLCamera camera;
PRGLTexture crate_texture;
PRGLMeshHandle cube_mesh;
struct PRGLGameObject cube_obj;

void primitives_init(void);
void primitives_update(void);
void primitives_draw_3d(void);
void primitives_draw_2d(void);
void primitives_cleanup(void);

int main(void)
{
    const char game_title[] = "Primitives Example";
    prgl_run_game(game_title, primitives_init, primitives_update,
                  primitives_draw_3d, primitives_draw_2d, primitives_cleanup);
}

void primitives_init(void)
{
    prgl_init_camera(&camera, 60.0f, 10.0f, PRGL_CAMERA_PROJECTION_PERSPECTIVE);

    crate_texture = prgl_load_texture("crate.png");
    cube_mesh = prgl_create_cube(crate_texture);
    prgl_init_game_object(&cube_obj, cube_mesh, (vec3){0.0f, 0.0f, -5.0f});
}

void primitives_update(void) { prgl_update_camera(&camera); }

void primitives_draw_3d(void) { prgl_draw_game_object_3d(&cube_obj); }

void primitives_draw_2d(void) {}

void primitives_cleanup(void)
{
    if (prgl_key_pressed(PRGL_KEY_ESCAPE))
    {
        if (cube_mesh != NULL)
        {
            prgl_delete_mesh(cube_mesh);
            cube_mesh = NULL;
        }
        prgl_close_game();
    }
}
