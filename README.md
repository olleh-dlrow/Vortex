# Vortex Engine



a 2D game engine program

## Support

windows10 x64



## Third Party

[spdlog](https://github.com/gabime/spdlog.git)

[glfw](https://github.com/glfw/glfw.git)

[imgui](https://github.com/ocornut/imgui.git)

[glm](https://github.com/g-truc/glm.git)



## Usage

git clone \<repo\> --recursive



**delete submodule:**

cd Vortex/vendor/moduleA

git rm moduleA

vim .git/config, delete [submodule moduleA]...

rm .git/modules/Vortex/vendor/moduleA



**add submodule:**

git submodule add url



premake:

find compiled files in cmake



## Vortex

main:

Initialize log



create application



loop application



delete application



### Log

#### CoreLogger

static member

shared_ptr

use in vortex

#### ClientLogger

static member

shared_ptr

use in sandbox

#### Init

set:

log name

log level: output all

output location: stdout and file



### Application

behaviour:

add layers

handle all layers' events

handle window events

main loop



static member:

s_Instance



member:

m_Window: Window

m_ImGuiLayer: ImGuiLayer

m_LastFrameTime: (maybe should put in another place)



state:

m_Running

m_Minimized



#### SandboxApp: Application

behaviour:

add user's layer

create application



### Window

virtual base class

behaviour:

set title, width, height

set sync

create main window



#### WindowsWindow: Window

behaviour:

init window's properties:

- response to window resize

- response to window close
- response to key press, release, repeat
- response to unicode char input
- response to mouse press, release
- response to scroll
- response to mouse move
- 

update window to flush graphics context



member:

GLFWWindow

GraphicsContext

WindowData:

- width, height
- title
- event callback, which response to glfw events



dependency:

glfw



### GraphicsContext

context of renderer

behaviour:

create graphics context

initialize context

swap buffers



#### OpenGLContext: GraphicsContext

context of opengl

behaviour:

initialize glfw context to OpenGLWindow and check opengl version

glfw swap buffers



### Event

the base of all kinds of events

member:

Handled bool

event type

> None = 0,
> WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
> AppTick, AppUpdate, AppRender,
> KeyPressed, KeyReleased, KeyTyped,
> MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled...

event name

event categories

> None = 0,
> EventCategoryApplication    = BIT(0),
> EventCategoryInput          = BIT(1),
> EventCategoryKeyboard       = BIT(2),
> EventCategoryMouse          = BIT(3),
> EventCategoryMouseButton    = BIT(4)

category:

application event:

- WindowResizeEvent
- WindowCloseEvent
- AppTickEvent
- AppUpdateEvent
- AppRenderEvent



input event:

key event:

- KeyEvent
- KeyPressedEvent
- KeyReleasedEvent
- KeyTypedEvent



mouse event:

- MouseMovedEvent
- MouseScrolledEvent
- MouseButtonEvent
- MouseButtonPressedEvent
- MouseButtonReleasedEvent



### EventDispatcher

dispatch current event to some handle

BROADCAST event to different classes



### Input

get input response like key, mouse etc.

behaviour:

get mouse position

check key pressed

check mouse button clicked



#### WindowsInput: Input

package glfw input event



### Layer

behaviour:

handle attach

handle detach

handle update

render gui

handle event



#### ImGuiLayer: Layer

initialize imgui context

config imgui



### LayerStack

store layers

LayerStack has two parts: Overlay on the top and Layer underneath it,
all of the layers in Overlay part are higher than layers in Layer part

behaviour:

push/pop layer

push/pop overlay



### Timestep

calculate time







