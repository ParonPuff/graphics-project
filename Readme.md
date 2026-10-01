|Criteria|Ratings|
|--------|-------|
|Camera movement|Pass|
|Projection|Pass|
|GUI|Needs additional work: The controls fro the perspective is far away from from the projection setting|
|Lightning|Needs additional work:The diffuse color does not handle the normals correctly when the light-source is behind the object. You may need to recompute the normals for the objects. Specular light lacks color|
|Objects|Pass|
|Texturing|Needs additional work: Complement the two part texture mapping with the second step intersecting it with the sphere in the computeUV method. Look in the slides and workshop 2|
|Code quality|Needs additional work Think about that the view, projection and model matrices does only need to be updated when they are changed|
|Other|Needs additional work: The resizing of the world looks strange when the window is resized. The objects are scale up when the window gets smaller. The world does not fill out the whole window if Make sure that you have added the resizing correctly. The view projection and model matrices does not need to be updated every frame only when they are changed|
