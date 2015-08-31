using System;
using GLUtil;

namespace Semo
{
	static class Program
	{
		static void Main(string[] args)
		{
			CSGLUT.Init(args);
			CSGLUT.CreateWindow("Assignment 1: Sean Moss");
			CSGLUT.MainLoop();
		}
	}
}
