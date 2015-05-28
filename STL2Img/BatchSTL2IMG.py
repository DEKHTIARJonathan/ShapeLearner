import os
import time
import vtk

def screenshot(filename):
    global renderWindow
    # screenshot code:
    w2if = vtk.vtkWindowToImageFilter()
    w2if.SetInput(renderWindow)
    w2if.Update()
    
    writer = vtk.vtkPNGWriter()
    writer.SetFileName(filename)
    writer.SetInput(w2if.GetOutput())
    writer.Write()

def computeSTL(subFolder, objName):
    global renderWindow
    inputFolder = "inputs"
    filename = inputFolder + "/" + subFolder + "/" + objName
    basename = os.path.basename(filename)[:-4]
 
    reader = vtk.vtkSTLReader()
    reader.SetFileName(filename)
    
    polydata = vtk.vtkPolyData()
    polydata = reader.GetOutput()
    polydata.Update()
    xmin, xmax, ymin, ymax, zmin, zmax = polydata.GetBounds()

    dx = (xmax + xmin) / 2
    dy = (ymax + ymin) / 2
    dz = (zmax + zmin) / 2
    
    mapper = vtk.vtkPolyDataMapper()
    if vtk.VTK_MAJOR_VERSION <= 5:
        mapper.SetInput(reader.GetOutput())
    else:
        mapper.SetInputConnection(reader.GetOutputPort())
        
    camera = vtk.vtkCamera()
    camera.SetPosition(0,0,1);
    camera.SetFocalPoint(0, 0, 0)
    camera.SetParallelProjection(1)
    
    # Create a rendering window and renderer
    renderer = vtk.vtkRenderer()
    renderer.SetActiveCamera(camera);
    renderWindow  = vtk.vtkRenderWindow()
    renderWindow.AddRenderer(renderer)
    renderWindow.SetSize(800,800)
    
    # create a renderwindowinteractor
    interactiveRenderer = vtk.vtkRenderWindowInteractor()
    interactiveRenderer.SetRenderWindow(renderWindow)
    
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetDiffuseColor(0,0,0)
    actor.SetOrigin(dx,dy,dz)
    
    # Assign actor to the renderer
    renderer.SetBackground(1,1,1)
    renderer.AddActor(actor)
    renderer.ResetCamera()

    renderWindow.Render()
    
    foldername = "outputs"
    outdir = foldername + "/" + subFolder

    if not os.path.isdir(outdir):
        os.makedirs(outdir) 

    outputName = outdir + "/" + basename + "_"

    outputExt = ".png"
    counter = 1

    for i in range(0, difficultyLevel):
        for x in range(0, difficultyArray[i]):
            renderer.ResetCameraClippingRange()
            screenshot(outputName + str(i) + "_" + str(x) + outputExt)
            actor.RotateZ(horizontalRotAngle[i])
            counter += 1
        actor.RotateX(verticalRotAngle)


def getDirs(path) : 
    dirs = []
    listing = os.listdir(path)
    for d in listing:
        if os.path.isdir(path + "/" + d):
            dirs.append(d)
    return dirs

def getFiles(path) : 
    files = []
    listing = os.listdir(path)
    for f in listing:
        if os.path.isfile(path + "/" + f):
            files.append(f)
    return files

####################### LAUNCH EXECUTION #######################

precision = ["Low", "Medium", "High", "Extreme", "Ultimate"]
precisionLevel = precision[4]

difficulty = dict()
difficulty[precision[0]] = [1,4,1]  # LOW
difficulty[precision[1]] = [1,3,4,3,1]  # MEDIUM
difficulty[precision[2]] = [1,3,6,8,6,3,1]  # HIGH
difficulty[precision[3]] = [1,3,5,6,8,6,5,3,1]  # EXTREME
difficulty[precision[4]] = [1,3,5,6,7,8,7,6,5,3,1]  # ULTIMATE

difficultyArray = difficulty[precisionLevel]

difficultyLevel = len(difficultyArray)
verticalRotAngle = 180 / float(difficultyLevel -1)
horizontalRotAngle = [float(360) / x for x in difficultyArray]

renderWindow  = vtk.vtkRenderWindow()
inputDir = "inputs"

subDirs = getDirs(inputDir)

for d in subDirs :
    files = getFiles(inputDir + "/" + d)
    for f in files :
        if f.endswith('.stl'):
            computeSTL(d,f)