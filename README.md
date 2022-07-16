# Vortex Engine

![image-20220717011307579](README/image-20220717011307579.png)

The logo above is painted by this renderer : )

a mini render engine used for studying and testing

## Support

windows10 x64



## Framework Abstract

see Wiki



## Plan

- write perspective and orthographic camera √
- import libigl, load models √
- modify GUI，move viewport into ImguiWindow with frame buffer √
- finish batch render of opengl √
- create entity √
- import imguizmo 
- interaction in viewport √
  - get world pos of cursor in viewport window √
  - select point √
  - drag point √
- implement geo algorithms
- implement render algorithms



## Note

use shared_ptr or unique_ptr to manage heap space

achieve first, others second

design first, code second



## Usage

```shell
git clone <repo> --recursive
.\GenerateProjects.bat
```



**delete submodule:**

```shell
cd Vortex/vendor/moduleA

git rm moduleA

vim .git/config, delete [submodule moduleA]...

rm .git/modules/Vortex/vendor/moduleA
```



**add submodule:**

```shell
git submodule add url
```
