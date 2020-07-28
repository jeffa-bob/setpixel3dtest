#include <vector>
#include <array>
#include <cmath>

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
    int r, g, b;
  };
  //array of three _3dvect that represents a triangle in 3d space, along with color
  struct tri
  {
    _3dvect tri[3];
    color col;
    _3dvect normal;
  };
  //one sphere with a location, radius, and color
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
    //direction of camera;
    ray camdir;
    // field of view in radians
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

  //changes first paramenter to the quotient of the first and second parameters
  void div_3dvect(_3dvect& fir, const _3dvect& sec)
  {
    fir.x /= sec.x;
    fir.y /= sec.y;
    fir.z /= sec.z;
  }


  //returns true if the first _3dvect is greater than the second on all axis
  bool greater_3dvect(_3dvect fir, _3dvect sec)
  {
    return fir.x > sec.x && fir.y > sec.y && fir.z > sec.z;
  }

  /*returns median of centrion of all tris in a mesh
  _3dvect centerofmesh(mesh current)
  {
    _3dvect final = { 0,0,0 };
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
  }*/

  //returns true if the end of a ray collids with a sphere;
  bool rayspherecollision(_3dvect rays, sphere sph)
  {
    return ((rays.x - sph.pos.x) * (rays.x - sph.pos.x)) + ((rays.y - sph.pos.y) * (rays.y - sph.pos.y)) + ((rays.z - sph.pos.z) * (rays.z - sph.pos.z)) < sph.radius * sph.radius;
  }

  //scales ray by unit vector of ray multiplied by magnitude; inplace
  void rayscaler(ray& rays, float mag, ray unitray) {
    rays = { {{unitray.raypoint[0]},{unitray.raypoint[1].x * mag,unitray.raypoint[1].y * mag,unitray.raypoint[1].z * mag}} };
  }
  //return the magnitute of a ray in a float, decimal will be rounded
  float magnitudeofaray(ray ray)
  {
    return (float)sqrt(((ray.raypoint[0].x - ray.raypoint[1].x) * (ray.raypoint[0].x - ray.raypoint[1].x)) + ((ray.raypoint[0].y - ray.raypoint[1].y) * (ray.raypoint[0].y - ray.raypoint[1].y)) + ((ray.raypoint[0].z - ray.raypoint[1].z) * (ray.raypoint[0].z - ray.raypoint[1].z)));
  }
  //gives unit vector of a ray as a ray from the first point of the ray
  ray unitvectorofray(ray& rays) {
    float magnitute = magnitudeofaray(rays);
    return { {{rays.raypoint[0]},{rays.raypoint[1].x / magnitute,rays.raypoint[1].y / magnitute,rays.raypoint[1].z / magnitute}} };
  }

  // dot product of two vectors
  float dotproduct(_3dvect n, _3dvect tri0) {
    return ((n.x * tri0.x) + (n.y * tri0.y) + (n.z * tri0.z));
  }

  // cross product of two vectors
  _3dvect crossproduct(_3dvect U, _3dvect V) {
    return { (U.y * V.z) - (U.z * V.y), (U.z * V.x) - (U.x * V.z), (U.x * V.y) - (U.y * V.x) };
  }

  //returns a ray of the trianges normal
  ray trinormal(tri& curtri) {
    _3dvect U = curtri.tri[1];
    sub_3dvect(U, curtri.tri[0]);
    _3dvect V = curtri.tri[2];
    sub_3dvect(V, curtri.tri[0]);
    ray nonunit = { {curtri.tri[0],crossproduct(U,V)} };
    return unitvectorofray(nonunit);
  }


  // returns true if a ray intersects a triangle
  bool intersecttri(tri& curtri, ray check) {
    _3dvect normal = curtri.normal;
    std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
    float d = dotproduct(normal, curtri.tri[0]);
    float t = ((dotproduct(normal, check.raypoint[0]) + d) / dotproduct(normal, check.raypoint[1]));
    if (t < 0) {
      return false;
    }
    _3dvect mult = check.raypoint[0];
    mul_3dvect(mult, { t,t,t });
    _3dvect P = check.raypoint[0];
    add_3dvect(P, mult);
    _3dvect edge0 = curtri.tri[1];
    sub_3dvect(edge0, curtri.tri[0]);
    _3dvect edge1 = curtri.tri[2];
    sub_3dvect(edge1, curtri.tri[1]);
    _3dvect edge2 = curtri.tri[0];
    sub_3dvect(edge2, curtri.tri[2]);
    _3dvect C0 = P;
    sub_3dvect(C0, curtri.tri[0]);
    _3dvect C1 = P;
    sub_3dvect(C1, curtri.tri[1]);
    _3dvect C2 = P;
    sub_3dvect(C2, curtri.tri[2]);
    if (dotproduct(normal, crossproduct(edge0, C0)) > 0 && dotproduct(normal, crossproduct(edge1, C1)) > 0 && dotproduct(normal, crossproduct(edge2, C2)) > 0) { return true; };
  }

  //shades points based on how far it is from the lightsource
  void shadepoint(color& objcolor, lightsource light, float magray) {
    if (magray <= light.brightness) {
      int brightness = 255 * (light.brightness / magray);
      mixcolor(objcolor, { brightness,brightness,brightness });
    }
    else {
      objcolor = { 0,0,0 };
    }
  }
  /*
  //sets the meshes detection sphere to the correct size
  void generatemeshsphere(mesh& curmesh) {
    _3dvect farplace;
    _3dvect cenmesh = centerofmesh(curmesh);
    for (tri& curtri : curmesh.mesh) {
      _3dvect diff;
      _3dvect centri = centeroftri(curtri);
      diff.x = abs(cenmesh.x - centri.x);
      diff.y = abs(cenmesh.y - centri.y);
      diff.z = abs(cenmesh.z - centri.z);
      if ( greater_3dvect(diff,farplace)) {
        farplace = diff;
      }
    }
    curmesh.checkbox.pos = cenmesh;
    curmesh.checkbox.radius = magnitudeofaray({ {cenmesh,farplace} })+0.2;
   }*/

   //class with list of objects to be rendered onto screen; buildarray function renders scene
  class currentworld
  {
  public:
    // handle to window
    HDC window;

    //vector of all of the meshes in the current world
    std::vector<tri> triworld;

    //the camera with width, height(in pixels), field of view, and distance that rays can go
    camera cam = { 600, 600, {0, 0}, {{{0, 0, 0}, {0, 1, 0}}}, 1.396263f };

    lightsource light;

    //builds the 2d pixel array of colors and displayes it to the screen
    void renderscreen()
    {
      //sphereworld.push_back({ light.pos,5,{light.brightness,light.brightness,light.brightness} });
      float halfwidth = tan(cam.fov / 2);
      for (int i = 0; i < cam.height; ++i)
      {
        for (int j = 0; j < cam.width; ++j)
        {
          float wideoffset = ((i * 2.0f / (cam.width - 1.0f)) - 1.0f) * halfwidth;
          float lengthoffset = ((j * 2.0f / (cam.width - 1.0f)) - 1.0f) * halfwidth;
          ray curray = { {{cam.pos},{1 * wideoffset,cam.camdir.raypoint[1].y,1 * lengthoffset}} };
          color raycol = willraycollide(curray);
          SetPixel(window, j, i, RGB(raycol.r, raycol.g, raycol.b));
        }
      }
    };
    //returns the color of the object the ray collides with else returns black;
    color willraycollide(const ray& rays)
    {
      ray increm = rays;
      ray uniincrem = unitvectorofray(increm);
      rayscaler(increm, 100, uniincrem);
      for (tri curtri : triworld) {
        bool tricheck = intersecttri(curtri, rays);
        //std::cout << std::boolalpha << tricheck << std::endl;
        if (tricheck) {
          return curtri.col;
        }
      }
      return { 0, 0, 0 };
    }
  };
}; // namespace world