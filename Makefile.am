bin_PROGRAMS = numptyphysics
noinst_LIBRARIES = libbox2d.a
INCLUDES = -IBox2D/Include

desktopentry_DATA = data/numptyphysics.desktop
serviceentry_DATA = data/numptyphysics.service
icon_26x26_DATA = data/icon26/numptyphysics.png 
#icon_40x40_DATA = data/icon40/numptyphysics.png
icon_64x64_DATA = data/icon64/numptyphysics.png
icon_scalable_DATA = data/icon64/numptyphysics.png

numptyphysics_SOURCES = \
	Path.cpp \
	Canvas.cpp \
	Font.cpp \
	Levels.cpp \
	ZipFile.cpp \
	Scene.cpp \
	Game.cpp \
	Script.cpp \
	App.cpp \
	Config.cpp \
	Http.cpp \
	Os.cpp \
	Ui.cpp \
	Dialogs.cpp \
	Event.cpp \
	Worker.cpp \
	happyhttp.cpp \
	zoomer.cpp \
	OsHildon.cpp \
	OsFreeDesktop.cpp \
	OsWin32.cpp

numptyphysics_CPPFLAGS = -IXX $(SDL_CFLAGS) $(HILDON_CFLAGS)
numptyphysics_LDADD = libbox2d.a $(SDL_LIBS) $(HILDON_LIBS)

numptyphysics_DATA = \
	data/C00_Title.npz \
	data/C01_Tutorial.npz \
	data/C10_Standard.npz \
	data/C50_Gesualdi.npz \
	data/L99_Gravity_Test.nph \
	data/edit.png \
	data/next.png \
	data/paper.jpg \
	data/pen.png \
	data/tick.png \
	data/blank.png \
	data/share.png \
	data/pause.png \
	data/play.png \
	data/reset.png \
	data/forward.png \
	data/help.png \
	data/choose.png \
	data/undo.png \
	data/close.png \
	data/record.png \
	data/theend.png \
	data/femkeklaver.ttf


libbox2d_a_SOURCES = \
	Box2D/Source/Collision/b2Distance.cpp \
	Box2D/Source/Collision/b2TimeOfImpact.cpp \
	Box2D/Source/Collision/b2CollideCircle.cpp \
	Box2D/Source/Collision/b2CollidePoly.cpp \
	Box2D/Source/Collision/Shapes/b2PolygonShape.cpp \
	Box2D/Source/Collision/Shapes/b2CircleShape.cpp \
	Box2D/Source/Collision/Shapes/b2Shape.cpp \
	Box2D/Source/Collision/b2PairManager.cpp \
	Box2D/Source/Collision/b2Collision.cpp \
	Box2D/Source/Collision/b2BroadPhase.cpp \
	Box2D/Source/Dynamics/b2WorldCallbacks.cpp \
	Box2D/Source/Dynamics/Joints/b2PrismaticJoint.cpp \
	Box2D/Source/Dynamics/Joints/b2MouseJoint.cpp \
	Box2D/Source/Dynamics/Joints/b2GearJoint.cpp \
	Box2D/Source/Dynamics/Joints/b2Joint.cpp \
	Box2D/Source/Dynamics/Joints/b2PulleyJoint.cpp \
	Box2D/Source/Dynamics/Joints/b2DistanceJoint.cpp \
	Box2D/Source/Dynamics/Joints/b2RevoluteJoint.cpp \
	Box2D/Source/Dynamics/Contacts/b2CircleContact.cpp \
	Box2D/Source/Dynamics/Contacts/b2PolyAndCircleContact.cpp \
	Box2D/Source/Dynamics/Contacts/b2Contact.cpp \
	Box2D/Source/Dynamics/Contacts/b2PolyContact.cpp \
	Box2D/Source/Dynamics/Contacts/b2ContactSolver.cpp \
	Box2D/Source/Dynamics/b2Island.cpp \
	Box2D/Source/Dynamics/b2Body.cpp \
	Box2D/Source/Dynamics/b2ContactManager.cpp \
	Box2D/Source/Dynamics/b2World.cpp \
	Box2D/Source/Common/b2BlockAllocator.cpp \
	Box2D/Source/Common/b2StackAllocator.cpp \
	Box2D/Source/Common/b2Settings.cpp \
	Box2D/Source/Common/b2Math.cpp

