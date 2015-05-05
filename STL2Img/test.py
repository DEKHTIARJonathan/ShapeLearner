# This simple example shows how to do basic rendering and pipeline
# creation.
 
import vtk
import os

difficultyArray = [1,6,8,6,1]
difficultyLevel = len(difficultyArray)
verticalRotAngle = 180 / float(difficultyLevel -1)
horizontalRotAngle = [float(360) / x for x in difficultyArray]

def screenshot(filename):
	# screenshot code:
	w2if = vtk.vtkWindowToImageFilter()
	w2if.SetInput(renderWindow)
	w2if.Update()
	 
	writer = vtk.vtkPNGWriter()
	writer.SetFileName(filename)
	writer.SetInputData(w2if.GetOutput())
	writer.Write()
	
def centerObj():
	polydata = vtk.vtkPolyData()
	polydata = reader.GetOutput()
	xmin, xmax, ymin, ymax, zmin, zmax = polydata.GetBounds()

	dx = (xmax + xmin) / float(2)
	dy = (ymax + ymin) / float(2)
	dz = (zmax + zmin) / float(2)
	
	actor.SetOrigin(dx,dy,dz)
	
renderer = vtk.vtkRenderer()
renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)

interactiveRenderer = vtk.vtkRenderWindowInteractor()
interactiveRenderer.SetRenderWindow(renderWindow)
	
filename = "rod2.stl"
reader = vtk.vtkSTLReader()
reader.SetFileName(filename)

mapper = vtk.vtkPolyDataMapper()
mapper.SetInputConnection(reader.GetOutputPort())

actor = vtk.vtkActor()
actor.SetMapper(mapper)
actor.GetProperty().SetDiffuseColor(0,0,0)
centerObj()
 
# Assign actor to the renderer
renderer.AddActor(actor)

renderer.SetBackground(1,1,1)
renderWindow.SetSize(800, 800)

renderWindow.Render()

outputName = "view_"
outputExt = ".png"
counter = 1

for i in range(0, difficultyLevel):
	for x in range(0, difficultyArray[i]):
		centerObj()
		screenshot(outputName + str(counter) + outputExt)
		actor.RotateZ(horizontalRotAngle[i])
		counter += 1
	actor.RotateX(verticalRotAngle)

interactiveRenderer.Initialize()
interactiveRenderer.Start()