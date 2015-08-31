using System;
using System.Diagnostics;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;

namespace GLUtil
{
	/// <summary>
	/// This class is designed to emulate the glut functions, since that is what is being used in the examples
	/// in the class. It is in charge of setting information about the window and context, similar to how
	/// GLUT works in C++.
	/// </summary>
	public static class CSGLUT
	{
		const int DEFAULT_WINDOW_WIDTH = 800;
		const int DEFAULT_WINDOW_HEIGHT = 600;

		#region Members
		private static GameWindow s_nativeWindow = null;
		// The native window wrapped in OpenTK
		public static GameWindow NativeWindow { get { return s_nativeWindow; } }

		public static bool IsWindowOpen { get { return s_nativeWindow != null; } }

		private static bool s_initialized = false;
		// If the library has been initialized
		public static bool Initialized { get { return s_initialized; } }

		// Registered callbacks
		private static LoadCallbackFunction s_loadCallback = delegate { };
		private static UpdateCallbackFunction s_updateCallback = delegate { };
		private static DisplayCallbackFunction s_displayCallback = delegate { };
		private static ResizeCallbackFunction s_resizeCallback = delegate { };

		// Time at last frame
		private static double s_lastTime = 0.0;
		#endregion

		// Mirrored functionality of glutInit(int argc, char** argv)
		public static void Init(string[] args)
		{
			if (Initialized)
				return;


		}

		// Mirrored functionality of glutCreateWindow()
		public static void CreateWindow(string title, int width = -1, int height = -1, int antialiasing = 0, bool fullscreen = false)
		{
			if (!Initialized)
				throw new InvalidOperationException("Cannot create a OpenGL window before calling CSGLUT.Init().");
			if (IsWindowOpen)
				throw new InvalidOperationException("Cannot open more than one OpenGL window.");

			if (width < 0) width = DEFAULT_WINDOW_WIDTH;
			if (height < 0) height = DEFAULT_WINDOW_HEIGHT;
			if (String.IsNullOrEmpty(title)) title = "OpenGL Window";
			if (antialiasing != 0 && antialiasing != 1 && antialiasing != 2 && antialiasing != 4)
				antialiasing = 0;

			// Create graphics mode with 32 bbp, 32 bit depth buffer, 8 bit stencil buffer, and supplied antialising samples
			GraphicsMode gMode = new GraphicsMode(new ColorFormat(8, 8, 8, 8), 32, 8, antialiasing);
			// GameWindowFlags
			GameWindowFlags windowFlags = fullscreen ? GameWindowFlags.Fullscreen : GameWindowFlags.Default;
			// GraphicsContextFlags
			GraphicsContextFlags contextFlags = GraphicsContextFlags.Default;

			s_nativeWindow = new GameWindow(width, height, gMode, title, windowFlags, DisplayDevice.Default, 2, 0, contextFlags);

			// Register the internal callbacks
			s_nativeWindow.Load += loadCallback;
			s_nativeWindow.UpdateFrame += updateCallback;
			s_nativeWindow.RenderFrame += displayCallback;
			s_nativeWindow.Resize += resizeCallback;
		}

		// Mirrored functionality of glutMainLoop()
		public static void MainLoop(double fps = 60.0)
		{
			if (!Initialized)
				throw new InvalidOperationException("Cannot launch the program main loop before calling CSGLUT.Init().");
			if (!IsWindowOpen)
				throw new InvalidOperationException("Cannot launch the program main loop before calling CSGLUT.CreateWindow().");

			s_nativeWindow.Run(fps);
		}

		// New functionality called when the window loads
		public static void LoadFunc(LoadCallbackFunction function)
		{
			if (function == null)
				throw new ArgumentNullException("function");

			s_loadCallback = function;
		}

		// Mirrored functionality of glutIdleFunc()
		public static void IdleFunc(UpdateCallbackFunction function)
		{
			if (function == null)
				throw new ArgumentNullException("function");

			s_updateCallback = function;
		}

		// Mirrored functionality of glutDisplayFunc()
		public static void DisplayFunc(DisplayCallbackFunction function)
		{
			if (function == null)
				throw new ArgumentNullException("function");

			s_displayCallback = function;
		}

		// Mirrored functionality of glutReshapeFunc()
		public static void ReshapeFunc(ResizeCallbackFunction function)
		{
			if (function == null)
				throw new ArgumentNullException("function");

			s_resizeCallback = function;
		}

		// Internal callback functions
		private static void loadCallback(object o, EventArgs e) { s_loadCallback(); }
		private static void updateCallback(object o, FrameEventArgs e)
		{
			
		}
		private static void displayCallback(object o, FrameEventArgs e)
		{

		}
		private static void resizeCallback(object o, EventArgs e) { s_resizeCallback(NativeWindow.Width, NativeWindow.Height); }
	}

	// Delegates for the callback functions
	public delegate void LoadCallbackFunction();
	public delegate void UpdateCallbackFunction(float elapsedTime);
	public delegate void DisplayCallbackFunction(float elapsedTime);
	public delegate void ResizeCallbackFunction(int width, int height);
}
