#include <vector>
#include <array>
#include <cmath>

namespace world
{
  //3 floats that represent a point in 3d space
  struct _3dpos
  {
    float x, y, z;
  };
  //2 _3dpos that represent a line in 3d space
  struct ray
  {
    _3dpos raypoint[2];
  };
  //a color of 3 values, red,green, and blue
  struct color
  {
    int r, g, b;
  };
  //array of three _3dpos that represents a triangle in 3d space, along with color
  struct tri
  {
    _3dpos tri[3];
    color col;
  };
  //one sphere with a location, radius, and color
  struct sphere
  {
    _3dpos pos;
    float radius;
    color col;
  };
  // source of light
  struct lightsource
  {
    _3dpos pos;
    color col;
    //brightness is distance to check for the lightsource for shading
    float brightness;
  };
  // camera view
  struct camera
  {
    int width, height;
    _3dpos pos;
    //direction of camera;
    ray camdir;
    // field of view in radians
    float fov;
  };

  //vector of tris and a sphere that is the checks if the mesh should be checked more closely to find what triangle a ray collides with
  struct mesh
  {
    std::vector<tri> mesh;
    sphere checkbox;
  };

  //sets first color to be average of both color
  void mixcolor(color& col1, color col2)
  {
    col1 = { col1.r + col2.r / 2, col1.g + col2.g / 2, col1.b + col2.b / 2 };
  };

  //returns true if both _3dpos are equal to each other
  bool _3dposequal(_3dpos pos1, _3dpos pos2)
  {
    return pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z;
  };

  //returns true if both _color are equal to each other
  bool colorequal(color col1, color col2)
  {
    return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b;
  }

  //returns centrion of tri
  _3dpos centeroftri(tri current)
  {
    _3dpos final;
    final.x = (current.tri[0].x + current.tri[1].x + current.tri[2].x) / 3;
    final.y = (current.tri[0].y + current.tri[1].y + current.tri[2].y) / 3;
    final.z = (current.tri[0].z + current.tri[1].z + current.tri[2].z) / 3;
    return final;
  }

  //changes first paramenter to the sum of the first and second parameters
  void add_3dpos(_3dpos& fir, _3dpos& sec)
  {
    fir.x += sec.x;
    fir.y += sec.y;
    fir.z += sec.z;
  }

  //changes first paramenter to the difference of the first and second parameters
  void sub_3dpos(_3dpos& fir, _3dpos sec)
  {
    fir.x -= sec.x;
    fir.y -= sec.y;
    fir.z -= sec.z;
  }

  //changes first paramenter to the product of the first and second parameters
  void mul_3dpos(_3dpos& fir, _3dpos& sec)
  {
    fir.x *= sec.x;
    fir.y *= sec.y;
    fir.z *= sec.z;
  }

  //changes first paramenter to the quotient of the first and second parameters
  void div_3dpos(_3dpos& fir, _3dpos& sec)
  {
    fir.x /= sec.x;
    fir.y /= sec.y;
    fir.z /= sec.z;
  }

  //returns true if the first _3dpos is greater than the second on all axis
  bool greater_3dpos(_3dpos fir, _3dpos sec)
  {
    return fir.x > sec.x && fir.y > sec.y && fir.z > sec.z;
  }

  //returns median of centrion of all tris in a mesh
  _3dpos centerofmesh(mesh current)
  {
    _3dpos final = {0,0,0};
    for (tri curtri : current.mesh)
    {
      final.x += centeroftri(curtri).x;
      final.y += centeroftri(curtri).y;
      final.z += centeroftri(curtri).z;
    }
    final.x /= current.mesh.size();
    final.y /= current.mesh.size();
    final.z /= current.mesh.size();
    return final;
  }

  //returns true if the end of a ray collids with a sphere;
  bool rayspherecollision(_3dpos rays, sphere sph)
  {
    bool x = rays.x < sph.pos.x + sph.radius && rays.x > sph.pos.x - sph.radius;
    bool y = rays.y < sph.pos.y + sph.radius && rays.y > sph.pos.y - sph.radius;
    bool z = rays.z < sph.pos.z + sph.radius && rays.z > sph.pos.z - sph.radius;
    return x && y && z;
  }

  //return the magnitute of a ray in a float, decimal will be rounded
  float magnitudeofaray(ray ray)
  {
    return (float)sqrt(pow(abs(ray.raypoint[0].x - ray.raypoint[1].x), 2) + pow(abs(ray.raypoint[0].y - ray.raypoint[1].y), 2) + pow(abs(ray.raypoint[0].z - ray.raypoint[1].z), 2));
  }

  //gives unit vector of a ray as a ray from the first point of the ray
  ray unitvectorofray(ray& rays) {
    float magnitute = magnitudeofaray(rays);
    return { {{rays.raypoint[0]},{rays.raypoint[1].x / magnitute,rays.raypoint[1].y / magnitute,rays.raypoint[1].z / magnitute}} };
  }

  //scales ray by unit vector of ray multiplied by magnitude; inplace
  void rayscaler(ray& rays, float mag) {
    ray unitray = unitvectorofray(rays);
    rays = { {{unitray.raypoint[0]},{unitray.raypoint[1].x * mag,unitray.raypoint[1].y * mag,unitray.raypoint[1].z * mag}} };
  }

  //class with list of objects to be rendered onto screen; buildarray function renders scene
  class currentworld
  {
  public:
    // handle to window
    HDC window;

    //vector of all of the meshes in the current world
    std::vector<mesh> meshworld;

    //vector of all of the spheres in the current world
    std::vector<sphere> sphereworld;

    //the camera with width, height(in pixels), field of view, and distance that rays can go
    camera cam = { 600, 600, {0, 0}, {{{0, 0, 0}, {0, 1, 0}}}, 1.396263f };

    lightsource light;

    //builds the 2d pixel array of colors and displayes it to the screen
    void renderscreen()
    {
      ray forward = { {{cam.pos},{cam.camdir.raypoint[1].x,cam.camdir.raypoint[1].y + 1,cam.camdir.raypoint[1].z} } };
      ray right = { {{cam.pos},{forward.raypoint[1].y,-forward.raypoint[1].y,forward.raypoint[1].z} } };
      float halfwidth = tan(cam.fov / 2);
      for (int i = 0; i < cam.height; ++i)
      {
        for (int j = 0; j < cam.width; ++j)
        {
          float wideoffset = ((i * 2.0 / (cam.width - 1.0)) - 1.0) * halfwidth;
          ray curray = { {{cam.pos},{1 * wideoffset,cam.camdir.raypoint[1].y,1 * wideoffset}} };
          color raycol = willraycollide(curray);
          SetPixel(window, j, i, RGB(raycol.r, raycol.g, raycol.b));
        }
      }
    };

    //returns the color of the object the ray collides with else returns black;
    color willraycollide(ray rays)
    {
      ray increm = rays;
      rayscaler(increm, 100);
      float scalee = 100.0f;
      while (scalee >= 1)
      {
        for (sphere& cursph : sphereworld)
        {
          if (rayspherecollision(increm.raypoint[1], cursph))
          {
            ray coltolight;
            coltolight.raypoint[0] = increm.raypoint[1];
            coltolight.raypoint[1] = light.pos;
            if (colorequal(willraycollide(coltolight), { 0, 0, 0 }))
            {
              mixcolor(cursph.col, light.col);
              return cursph.col;
            }
            else
            {
              mixcolor(cursph.col, { 40,40,40 });
              return cursph.col;
            }
          }
        }
        scalee -= 0.5;
        rayscaler(increm, scalee);
      }
      return { 0, 0, 0 };
    }
  };
}; // namespace world