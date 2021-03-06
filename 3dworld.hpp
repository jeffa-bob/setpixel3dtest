#include <vector>
#include <array>
#include <math.h>

namespace world
{
  //3 floats that represent a point in 3d space
  struct _3dvect
  {
    float x, y, z;
  };
  //2 _3dvect that represent a line in 3d space
  struct ray
  {
    _3dvect raypoint[2];
  };
  //a color of 3 values, red,green, and blue
  struct color
  {
    int b, g, r;
  };
  //array of three _3dvect that represents a triangle in 3d space, along with color
  struct tri
  {
    _3dvect tri[3];
    color col;
    _3dvect normal;
  };
  struct sphere
  {
    _3dvect pos;
    float radius;
  };
  // source of light
  struct lightsource
  {
    _3dvect pos;
    color col;
    //brightness is distance to check for the lightsource for shading
    int brightness;
  };
  // camera view
  struct camera
  {
    int width, height;
    _3dvect pos;
    ray camdir;
    float fov;
  };

  /*vector of tris and a sphere that is the checks if the mesh should be checked more closely to find what triangle a ray collides with
  struct mesh
  {
    std::vector<tri> mesh;
  };
  */
  //sets first color to be average of both color
  void mixcolor(color& col1, color col2)
  {
    col1 = { col1.r + col2.r / 2, col1.g + col2.g / 2, col1.b + col2.b / 2 };
  };

  //returns true if both _3dvect are equal to each other
  bool _3dvectequal(_3dvect pos1, _3dvect pos2)
  {
    return pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z;
  };

  //returns true if both _color are equal to each other
  bool colorequal(color col1, color col2)
  {
    return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b;
  }

  //returns centrion of tri
  _3dvect centeroftri(tri current)
  {
    _3dvect final;
    final.x = (current.tri[0].x + current.tri[1].x + current.tri[2].x) / 3;
    final.y = (current.tri[0].y + current.tri[1].y + current.tri[2].y) / 3;
    final.z = (current.tri[0].z + current.tri[1].z + current.tri[2].z) / 3;
    return final;
  }

  //changes first paramenter to the sum of the first and second parameters
  void add_3dvect(_3dvect& fir, const _3dvect& sec)
  {
    fir.x += sec.x;
    fir.y += sec.y;
    fir.z += sec.z;
  }

  //changes first paramenter to the difference of the first and second parameters
  void sub_3dvect(_3dvect& fir, const _3dvect sec)
  {
    fir.x -= sec.x;
    fir.y -= sec.y;
    fir.z -= sec.z;
  }

  //changes first paramenter to the product of the first and second parameters
  void mul_3dvect(_3dvect& fir, const _3dvect sec)
  {
    fir.x *= sec.x;
    fir.y *= sec.y;
    fir.z *= sec.z;
  }

  //returns true if the first _3dvect is greater than the second on all axis
  bool greater_3dvect(_3dvect fir, _3dvect sec)
  {
    return fir.x > sec.x && fir.y > sec.y && fir.z > sec.z;
  }

  //returns true if the end of a ray collids with a sphere;
  bool rayspherecollision(_3dvect rays, sphere sph)
  {
    return ((rays.x - sph.pos.x) * (rays.x - sph.pos.x)) + ((rays.y - sph.pos.y) * (rays.y - sph.pos.y)) + ((rays.z - sph.pos.z) * (rays.z - sph.pos.z)) < sph.radius * sph.radius;
  }

  //scales ray by unit vector of ray multiplied by magnitude; inplace
  void rayscaler(ray &rays, float mag, ray unitray)
  {
    rays = {{{unitray.raypoint[0]}, {unitray.raypoint[1].x * mag, unitray.raypoint[1].y * mag, unitray.raypoint[1].z * mag}}};
  }
  //return the magnitute of a ray in a float, decimal will be rounded
  float magnitudeofaray(ray ray)
  {
    return (float)sqrt(((ray.raypoint[0].x - ray.raypoint[1].x) * (ray.raypoint[0].x - ray.raypoint[1].x)) + ((ray.raypoint[0].y - ray.raypoint[1].y) * (ray.raypoint[0].y - ray.raypoint[1].y)) + ((ray.raypoint[0].z - ray.raypoint[1].z) * (ray.raypoint[0].z - ray.raypoint[1].z)));
  }
  //gives unit vector of a ray as a ray from the first point of the ray
  ray unitvectorofray(ray &rays)
  {
    float magnitute = magnitudeofaray(rays);
    return {{{rays.raypoint[0]}, {rays.raypoint[1].x / magnitute, rays.raypoint[1].y / magnitute, rays.raypoint[1].z / magnitute}}};
  }

  // dot product of two vectors
  float dotproduct(_3dvect n, _3dvect tri0)
  {
    return ((n.x * tri0.x) + (n.y * tri0.y) + (n.z * tri0.z));
  }

  // cross product of two vectors
  _3dvect crossproduct(_3dvect U, _3dvect V)
  {
    return {((U.y * V.z) - (U.z * V.y)), ((U.z * V.x) - (U.x * V.z)), ((U.x * V.y) - (U.y * V.x))};
  }

  //returns a ray of the trianges normal
  ray trinormal(tri &curtri)
  {
    _3dvect U  = curtri.tri[1];
    sub_3dvect(U, curtri.tri[0]);
    _3dvect V = curtri.tri[2];
    sub_3dvect(V, curtri.tri[0]);
    ray nonunit = {{curtri.tri[0], crossproduct(U, V)}};
    return nonunit;
  }

  // returns true if a ray intersects a triangle
  bool intersecttri(tri& curtri, ray check)
  {
    _3dvect normal = curtri.normal;
    float d = dotproduct(normal, curtri.tri[0]);
    float t = ((dotproduct(normal, check.raypoint[0]) + d) / dotproduct(normal, check.raypoint[1]));
    if (t < 0) {
      return false;
    }
    _3dvect p = check.raypoint[0];
    _3dvect tvect = { t, t, t };
    mul_3dvect(tvect, check.raypoint[1]);
    add_3dvect(p, tvect);
    //std::cout << p.x << " " << p.y << " " << p.z << std::endl;

    //sub_3dvect(normal, curtri.tri[0]);
    _3dvect edge0 = curtri.tri[1];
    sub_3dvect(edge0, curtri.tri[0]);
    _3dvect C0 = p;
    sub_3dvect(C0, curtri.tri[0]);
    _3dvect c = crossproduct(edge0, C0);
    float h = dotproduct(normal, c);
    if (h < 0)return false;

    edge0 = curtri.tri[2];
    sub_3dvect(edge0, curtri.tri[1]);
    C0 = p;
    sub_3dvect(C0, curtri.tri[1]);
    c = crossproduct(edge0, C0);
    h = dotproduct(normal, c);
    if (h < 0)return false;

    edge0 = curtri.tri[0];
    sub_3dvect(edge0, curtri.tri[2]);
    C0 = p;
    sub_3dvect(C0, curtri.tri[2]);
    c = crossproduct(edge0, C0);
    h = dotproduct(normal, c);
    if ( h < 0)return false;
    return true;
  }
  float normalizedot(_3dvect u, _3dvect v){
    float x = dotproduct(u, v);
    return abs(x/(magnitudeofaray({{v,{0, 0, 0}}}) * (magnitudeofaray({ {u,{0, 0, 0}} }))));
  }
  //class with list of objects to be rendered onto screen; buildarray function renders scene
  class currentworld
  {
  public:
    // handle to window
    HDC window;

    //vector of all of the meshes in the current world
    std::vector<tri> triworld;

    //the camera with width, height(in pixels), field of view, and distance that rays can go
    camera cam = {600, 600, {0, 0, 0}, {{cam.pos, {0, 1, 0}}}, 1.396263f};

    lightsource light;

    //builds the 2d pixel array of colors and displayes it to the screen
    void renderscreen()
    {
      _3dvect adjustdir = cam.camdir.raypoint[1];
      sub_3dvect(adjustdir, cam.pos);
      COLORREF* pixelarr = (COLORREF*)calloc(600 * 600, sizeof(COLORREF));
      float halfwidth = tan(cam.fov / 2);
      for (int i = 0; i < cam.height; ++i)
      {
        for (int j = 0; j < cam.width; ++j)
        {
          float wideoffset = ((i * 2.0f / (cam.width - 1.0f)) - 1.0f) * halfwidth;
          float lengthoffset = ((j * 2.0f / (cam.width - 1.0f)) - 1.0f) * halfwidth;
          ray curray = {{{cam.pos}, {1 * wideoffset, adjustdir.y, 1 * lengthoffset}}};
          color raycol = willraycollide(curray);
          pixelarr[cam.width*i+j] = RGB(raycol.r, raycol.g, raycol.b);
        }
      }
      HDC src = CreateCompatibleDC(window);
      HBITMAP map = CreateBitmap(cam.width, cam.height, 1, 32, (void*) pixelarr);
      SelectObject(src, map);
      BitBlt(window, 0, 0, cam.height, cam.width, src,0,0,SRCCOPY );
      free(pixelarr);
      DeleteObject(map);
      DeleteDC(src);
    };

    void setcamangle(float curangle) {
      cam.camdir.raypoint[1].y = sinf(curangle);
      cam.camdir.raypoint[1].x = cosf(curangle);
      add_3dvect(cam.camdir.raypoint[1], cam.pos);
    }

    //returns the color of the object the ray collides with else returns black;
    color willraycollide(const ray &rays)
    {
      ray increm = rays;
      for (tri curtri : triworld)
      {
        bool tricheck = intersecttri(curtri, rays);
        if (tricheck)
        {
          _3dvect adjustray = increm.raypoint[1];
          sub_3dvect(adjustray, increm.raypoint[0]);
          _3dvect adjustnorm = curtri.normal;
          sub_3dvect(adjustnorm,curtri.tri[0]);
          float greyfact = normalizedot(adjustray, adjustnorm);
          return {(int)(curtri.col.r*greyfact),(int)(curtri.col.g * greyfact),(int)(curtri.col.b * greyfact)};
        }
      }
      return {0, 0, 0};
    }
  };
}; // namespace world