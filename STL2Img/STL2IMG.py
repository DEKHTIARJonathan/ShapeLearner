import os
import vtk

os.chdir("C:\Users\dekhtiar\Desktop\Git Projects\ShapeLearner\STL2Img")

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
    writer.SetInput(w2if.GetOutput())
    writer.Write()


filename = "rod2.stl"
 
reader = vtk.vtkSTLReader()
reader.SetFileName(filename)

polydata = vtk.vtkPolyData()
polydata = reader.GetOutput()
xmin, xmax, ymin, ymax, zmin, zmax = polydata.GetBounds()

dx = (xmax + xmin) / 2
dy = (ymax + ymin) / 2
dz = (zmax + zmin) / 2

mapper = vtk.vtkPolyDataMapper()
if vtk.VTK_MAJOR_VERSION <= 5:
    mapper.SetInput(reader.GetOutput())
else:
    mapper.SetInputConnection(reader.GetOutputPort())

actor = vtk.vtkActor()
actor.SetMapper(mapper)
actor.GetProperty().SetDiffuseColor(0,0,0)
actor.SetOrigin(dx,dy,dz)

camera = vtk.vtkCamera ();
camera.SetPosition(0, 0,100)
camera.SetFocalPoint(0, 0, 0)
camera.SetParallelProjection(1)

# Create a rendering window and renderer
renderer = vtk.vtkRenderer()
renderer.SetActiveCamera(camera);

# Assign actor to the renderer
renderer.SetBackground(1,1,1)
renderer.AddActor(actor)
renderer.ResetCamera()

renderWindow = vtk.vtkRenderWindow()
renderWindow.SetOffScreenRendering(1)
renderWindow.AddRenderer(renderer)
renderWindow.SetSize(800, 800)
renderWindow.Render()

outputName = "view_"
outputExt = ".png"
counter = 1

'''

for i in range(0, difficultyLevel):
    for x in range(0, difficultyArray[i]):
        screenshot(outputName + str(counter) + outputExt)
        actor.RotateZ(horizontalRotAngle[i])
        counter += 1
    actor.RotateX(verticalRotAngle)
'''

# enable user interface interactor
interactiveRenderer = vtk.vtkRenderWindowInteractor()
interactiveRenderer.SetRenderWindow(renderWindow)
interactiveRenderer.Initialize()
interactiveRenderer.Start()