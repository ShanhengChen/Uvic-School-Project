
var canvas;
var gl;

var program;

var near = 5;
var far = 100;


var left = -6.0;
var right = 6.0;
var ytop =6.0;
var bottom = -6.0;


var lightPosition2 = vec4(100.0, 100.0, 100.0, 1.0 );
var lightPosition = vec4(0.0, 0.0, 100.0, 1.0 );

var lightAmbient = vec4(0.2, 0.2, 0.2, 1.0 );
var lightDiffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
var lightSpecular = vec4( 1.0, 1.0, 1.0, 1.0 );

var materialAmbient = vec4( 1.0, 0.0, 1.0, 1.0 );
var materialDiffuse = vec4( 1.0, 0.8, 0.0, 1.0 );
var materialSpecular = vec4( 0.4, 0.4, 0.4, 1.0 );
var materialShininess = 30.0;

var ambientColor, diffuseColor, specularColor;

var modelMatrix, viewMatrix, modelViewMatrix, projectionMatrix, normalMatrix;
var modelViewMatrixLoc, projectionMatrixLoc, normalMatrixLoc;
var eye;
var at = vec3(0.0, 0.0, 0.0);
var up = vec3(0.0, 1.0, 0.0);

var RX = 0;
var RY = 0;
var RZ = 0;

var MS = []; // The modeling matrix stack
var TIME = 0.0; // Realtime
var dt = 0.0
var prevTime = 0.0;
var resetTimerFlag = true;
var animFlag = false;
var controller;

// These are used to store the current state of objects.
// In animation it is often useful to think of an object as having some DOF (Depth of field)
// Then the animation is simply evolving those DOF over time.
var sphereRotation = [0,0,0];
var spherePosition = [-4,0,0];

var cubeRotation = [0,0,0];
var cubePosition = [-1,0,0];

var cylinderRotation = [0,0,0];
var cylinderPosition = [1.1,0,0];

var coneRotation = [0,0,0];
var conePosition = [3,0,0];

// Setting the colour which is needed during illumination of a surface
function setColor(c)
{
    ambientProduct = mult(lightAmbient, c);
    diffuseProduct = mult(lightDiffuse, c);
    specularProduct = mult(lightSpecular, materialSpecular);
    
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "ambientProduct"),flatten(ambientProduct) );
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "diffuseProduct"),flatten(diffuseProduct) );
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "specularProduct"),flatten(specularProduct) );
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "lightPosition"),flatten(lightPosition) );
    gl.uniform1f( gl.getUniformLocation(program, 
                                        "shininess"),materialShininess );
}

window.onload = function init() {

    canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport( 0, 0, canvas.width/2, canvas.height );
    gl.clearColor( 0.5, 0.5, 1.0, 1.0 );
    
    gl.enable(gl.DEPTH_TEST);

    //
    //  Load shaders and initialize attribute buffers
    //
    program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );
    

    setColor(materialDiffuse);
	
	// Initialize some shapes, note that the curved ones are procedural which allows you to parameterize how nice they look
	// Those number will correspond to how many sides are used to "estimate" a curved surface. More = smoother
    Cube.init(program);
    Cylinder.init(20,program);
    Cone.init(20,program);
    Sphere.init(36,program);

    // Matrix uniforms
    modelViewMatrixLoc = gl.getUniformLocation( program, "modelViewMatrix" );
    normalMatrixLoc = gl.getUniformLocation( program, "normalMatrix" );
    projectionMatrixLoc = gl.getUniformLocation( program, "projectionMatrix" );
    
    // Lighting Uniforms
    gl.uniform4fv( gl.getUniformLocation(program, 
       "ambientProduct"),flatten(ambientProduct) );
    gl.uniform4fv( gl.getUniformLocation(program, 
       "diffuseProduct"),flatten(diffuseProduct) );
    gl.uniform4fv( gl.getUniformLocation(program, 
       "specularProduct"),flatten(specularProduct) );	
    gl.uniform4fv( gl.getUniformLocation(program, 
       "lightPosition"),flatten(lightPosition) );
    gl.uniform1f( gl.getUniformLocation(program, 
       "shininess"),materialShininess );


    document.getElementById("animToggleButton").onclick = function() {
        if( animFlag ) {
            animFlag = false;
        }
        else {
            animFlag = true;
            resetTimerFlag = true;
            window.requestAnimFrame(render);
        }
        //console.log(animFlag);
		
		controller = new CameraController(canvas);
		controller.onchange = function(xRot,yRot) {
			RX = xRot;
			RY = yRot;
			window.requestAnimFrame(render); };
    };

    render(0);
}

// Sets the modelview and normal matrix in the shaders
function setMV() {
    modelViewMatrix = mult(viewMatrix,modelMatrix);
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix) );
    normalMatrix = inverseTranspose(modelViewMatrix);
    gl.uniformMatrix4fv(normalMatrixLoc, false, flatten(normalMatrix) );
}

// Sets the projection, modelview and normal matrix in the shaders
function setAllMatrices() {
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix) );
    setMV();   
}

// Draws a 2x2x2 cube center at the origin
// Sets the modelview matrix and the normal matrix of the global program
// Sets the attributes and calls draw arrays
function drawCube() {
    setMV();
    Cube.draw();
}

// Draws a sphere centered at the origin of radius 1.0.
// Sets the modelview matrix and the normal matrix of the global program
// Sets the attributes and calls draw arrays
function drawSphere() {
    setMV();
    Sphere.draw();
}

// Draws a cylinder along z of height 1 centered at the origin
// and radius 0.5.
// Sets the modelview matrix and the normal matrix of the global program
// Sets the attributes and calls draw arrays
function drawCylinder() {
    setMV();
    Cylinder.draw();
}

// Draws a cone along z of height 1 centered at the origin
// and base radius 1.0.
// Sets the modelview matrix and the normal matrix of the global program
// Sets the attributes and calls draw arrays
function drawCone() {
    setMV();
    Cone.draw();
}

// Post multiples the modelview matrix with a translation matrix
// and replaces the modeling matrix with the result
function gTranslate(x,y,z) {
    modelMatrix = mult(modelMatrix,translate([x,y,z]));
}

// Post multiples the modelview matrix with a rotation matrix
// and replaces the modeling matrix with the result
function gRotate(theta,x,y,z) {
    modelMatrix = mult(modelMatrix,rotate(theta,[x,y,z]));
}

// Post multiples the modelview matrix with a scaling matrix
// and replaces the modeling matrix with the result
function gScale(sx,sy,sz) {
    modelMatrix = mult(modelMatrix,scale(sx,sy,sz));
}
function gtransfer(sx,sy,sz){
	modelMatrix = mult(modelMatrix,translate(sx,sy,sz));
}

// Pops MS and stores the result as the current modelMatrix
function gPop() {
    modelMatrix = MS.pop();
}

// pushes the current modelViewMatrix in the stack MS
function gPush() {
    MS.push(modelMatrix);
}


function render(timestamp) {
    
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    
    eye = vec3(0,0,10);
    MS = []; // Initialize modeling matrix stack
	
	// initialize the modeling matrix to identity
    modelMatrix = mat4();
    
    // set the camera matrix
    viewMatrix = lookAt(eye, at , up);
   
    // set the projection matrix
    projectionMatrix = ortho(left, right, bottom, ytop, near, far);
    
    
    // set all the matrices
    setAllMatrices();

	//
	var cubetran = 0;
    
	if( animFlag )
    {
		// dt is the change in time or delta time from the last frame to this one
		// in animation typically we have some property or degree of freedom we want to evolve over time
		// For example imagine x is the position of a thing.
		// To get the new position of a thing we do something called integration
		// the simpelst form of this looks like:
		// x_new = x + v*dt
		// That is the new position equals the current position + the rate of of change of that position (often a velocity or speed), times the change in time
		// We can do this with angles or positions, the whole x,y,z position or just one dimension. It is up to us!
		dt = (timestamp - prevTime) / 1000.0;
		prevTime = timestamp;
	}

	gPush();{
	gTranslate(0,6,0);
		drawCube();
	}
	gPop();



	gPush();{
		gTranslate(0,-6,0);
		setColor(vec4(0.0,0.0,0.0,1.0));
		gPush();{ 
			gScale(6.0,1.0,1.0);
			drawCube();
		}gPop();
	}gPop();

	// bigger rock
	gPush();
		gTranslate(0,-4.3,spherePosition[2]);
		gPush();
		{
			gScale(0.7,0.7,0.7);
			setColor(vec4(0.33,0.33,0.33,1.0));
			drawSphere();
		}
		gPop();
	gPop();
	// smaller rock
	gPush();
	gTranslate(-0.95,-4.7,spherePosition[2]);
	gPush();
	{
		gScale(0.3,0.3,0.3);
		setColor(vec4(0.33,0.33,0.33,1.0));
		drawSphere();
	}
	gPop();
gPop();

	// human
	gPush();{ 
		gTranslate(4.2,2,1);
		gRotate(-30,0,1,0);
		coneRotation[1] = 0.25*Math.cos(0.002*timestamp + 1);
		gTranslate(coneRotation[1],coneRotation[1],0);
		setColor(vec4(0.6,0.2,1,1.0));
		gPush();
		{
			//human's head
			gScale(0.4,0.4,0.4);
			drawSphere();
		}
		gPop();
		// human's body
		gPush();{ 
		gTranslate(0,-1.5,0);
		gScale(0.75,1.1,0.3);
		drawCube();
		}gPop();

		gTranslate(0,-1.5,0);
		gScale(0.75,1.1,0.4);

		gPush();{ 
			// one of human's thigh
			coneRotation[2] = 6*Math.cos(0.0025*timestamp + 1);
			gRotate(coneRotation[2],1,0,0);	
			gRotate(-30,-1,0,0);
			gTranslate(-0.5,-1.2,0.7);
			gPush();{
			gScale(0.2,0.5,0.3);
			drawCube();
			}gPop();
			//one of human's calf
			gRotate(-40,-1,0,0);
			gTranslate(-0.05,-1.55,0.3);
			gPush();{
			gScale(0.2,1,0.2);
			drawCube();
			}gPop();
			gTranslate(0,-1,0.1);
			gScale(0.2,0.2,0.5);
			drawCube();
			
		}gPop();
			// another one of human's thigh
		gPush();{ 
			coneRotation[2] = 6*Math.cos(0.0025*timestamp + 1);
			gRotate(coneRotation[2],-1,0,0);
			gRotate(-40,-1,0,0);
			gTranslate(0.5,-1.2,1);
			gPush();{
			gScale(0.2,0.5,0.3);
			drawCube();
			}gPop();
			// another one of human's calf
			gRotate(-40,-1,0,0);
			gTranslate(-0.05,-1.55,0.3);
			gPush();{
			gScale(0.2,1,0.2);
			drawCube();
			}gPop();
			gTranslate(0,-1,0.1);
			gScale(0.2,0.2,0.5);
			drawCube();	
		}gPop();
	
	//------- bubbles
	gTranslate(0.3,0.4,0);
	coneRotation[0] += 10*dt;
		for (var i = 0; i < 4; i++){
		gPush();{
			gScale(0.15,0.1,0.2);
			gTranslate(0,coneRotation[0]+ i*1.5,1);
			gPush();{
			setColor(vec4(1,1,1,1.0));
			drawSphere();
			}gPop();
		}gPop();
	}	
	}gPop();

	// fish
	gPush();{
		gTranslate(-0,-2.0,1);
		gPush();
		{
			setColor(vec4(0.6,0.6,0.5,1.0));

			gScale(0.7,0.7,1);
			cylinderRotation[1] = cylinderRotation[1] + 60*dt;
			gRotate(cylinderRotation[1],0,-1,0);
			gTranslate(230*dt,0,1); // swimming path length

			cylinderPosition [1] += 0.06*Math.cos(0.002*timestamp + 1);
			gTranslate(0,cylinderPosition [1],0); // swimming up and down
			drawCone(); // fish head
			gPush();{ // one of eye ball
				setColor(vec4(1.0,1.0,1.0,1.0));
				gTranslate(0.4,0.6,0.0);
				gScale(0.2,0.2,0.15);
				drawSphere();
				gPush();{ // one of eye pupil
					setColor(vec4(0.0,0.0,0.0,1.0));
					gTranslate(0.0,0.0,0.2);
					gScale(0.6,0.6,1);
					drawSphere();
				}gPop();
			}gPop();
			gPush();{ // another one of eye ball
				setColor(vec4(1.0,1.0,1.0,1.0));
				gTranslate(-0.4,0.6,0.0);
				gScale(0.2,0.2,0.15);
				drawSphere();
				gPush();{ // another one of eye pupils
					setColor(vec4(0.0,0.0,0.0,1.0));
					gTranslate(0.0,0.0,0.2);
					gScale(0.6,0.6,1);
					drawSphere();
				}gPop();
			}gPop();
			// fish body
			setColor(vec4(0.6,0,0.4,1.0));
			gTranslate(0,0.0,-1.5);
			gPush();{ 
			gRotate(180,0,1,0);
			gScale(1,1,2);
			drawCone();
			}gPop();
			// one of tail fin
			gTranslate(0,0.0,-1.49);
			gPush();{ 
			cubeRotation[2] = 1.2*Math.sin(0.02*timestamp + 1); // upper fin wagging
			gRotate(coneRotation[2]*8,0,0,1);
				gTranslate(0,0.5,0.1);
				gRotate(125,-1,0,0);
				gScale(0.3,0.3,1.2);
				drawCone();
				gTranslate(0,0.3,0.3);
			}gPop();
			// another one of tail fin
			gPush();{
			cubeRotation[2] = 1.2*Math.sin(0.02*timestamp + 1); // down fin wagging
			gRotate(coneRotation[2]*8,0,0,-1); 
				gTranslate(0,-0.4,0.1);
				gRotate(125,1,0,0);
				gScale(0.3,0.3,0.8);
				drawCone();
			}gPop();
		}gPop();
	}gPop();

//seaweed
gPush();{
	gTranslate(0,-3.2,0);
	setColor(vec4(0,0.6,0.1,1.0));
	gPush();{ // first middle seaweed
		gScale(0.15,0.4,0.3); 
		drawSphere();
	}gPop();
	gTranslate(0,-0.05,0);	
	gPush();{
	sphereRotation[1] = 3*Math.cos(0.002*timestamp + 1);
	gRotate(sphereRotation[1],0,0,1); 
	var temp = -1;
	for(var i = 0; i < 9; i++){ // rest of middle seaweed (9 in total)
		if (i % 2 == 0){temp *= -1; gRotate(1*temp*sphereRotation[1],0,0,1);}
		gRotate(12*temp,0,0,1);
		gTranslate(0.4*Math.tan(12*temp/180 *Math.PI),0.8-0.05,0);
		gPush();{ 
			gScale(0.15,0.4,0.3);
			drawSphere();
		}gPop();}
	}gPop();

	gTranslate(-0.6,-0.7,-0.4);
	gPush();{ // first left seaweed
		gScale(0.15,0.4,0.3);
		drawSphere();
	}gPop();
	gTranslate(0,-0.05,0);	
	gPush();{
		sphereRotation[1] = 4*Math.cos(0.002*timestamp + 1);
		gRotate(sphereRotation[1],0,0,1); 
		var temp = -0.75;
		for(var i = 0; i < 9; i++){ // rest of middle seaweed (9 in total)
			if (i % 3 == 0){temp *= -1; gRotate(1.33*temp*sphereRotation[1],0,0,1);}
			gRotate(12*temp,0,0,1);
			gTranslate(0.4*Math.tan(12*temp/180 *Math.PI),0.8-0.05,0);
			gPush();{ 
				gScale(0.15,0.4,0.3);
				drawSphere();
			}gPop();}
		}gPop();
	
	gTranslate(+1.2,0,0);
	gPush();{ 	// first right seaweed
		gScale(0.15,0.4,0.3);
		drawSphere();
	}gPop();
	gTranslate(0,-0.05,0);	
	gPush();{
		sphereRotation[1] = 3*Math.cos(0.002*timestamp + 1);
		gRotate(sphereRotation[1],0,0,1); 
		var temp = -0.6;
		for(var i = 0; i < 9; i++){ // rest of right seaweed (9 in total)
			if (i % 2 == 0){temp *= -1; gRotate(1*temp*sphereRotation[1],0,0,1);}
			gRotate(15*temp,0,0,1);
			gTranslate(0.4*Math.tan(15*temp/180 *Math.PI),0.8-0.05,0);
			gPush();{ 
				gScale(0.15,0.4,0.3);
				drawSphere();
			}gPop();}
		}gPop();
}gPop();



    if( animFlag )
        window.requestAnimFrame(render);
}

// A simple camera controller which uses an HTML element as the event
// source for constructing a view matrix. Assign an "onchange"
// function to the controller as follows to receive the updated X and
// Y angles for the camera:
//
//   var controller = new CameraController(canvas);
//   controller.onchange = function(xRot, yRot) { ... };
//
// The view matrix is computed elsewhere.
function CameraController(element) {
	var controller = this;
	this.onchange = null;
	this.xRot = 0;
	this.yRot = 0;
	this.scaleFactor = 3.0;
	this.dragging = false;
	this.curX = 0;
	this.curY = 0;
	
	// Assign a mouse down handler to the HTML element.
	element.onmousedown = function(ev) {
		controller.dragging = true;
		controller.curX = ev.clientX;
		controller.curY = ev.clientY;
	};
	
	// Assign a mouse up handler to the HTML element.
	element.onmouseup = function(ev) {
		controller.dragging = false;
	};
	
	// Assign a mouse move handler to the HTML element.
	element.onmousemove = function(ev) {
		if (controller.dragging) {
			// Determine how far we have moved since the last mouse move
			// event.
			var curX = ev.clientX;
			var curY = ev.clientY;
			var deltaX = (controller.curX - curX) / controller.scaleFactor;
			var deltaY = (controller.curY - curY) / controller.scaleFactor;
			controller.curX = curX;
			controller.curY = curY;
			// Update the X and Y rotation angles based on the mouse motion.
			controller.yRot = (controller.yRot + deltaX) % 360;
			controller.xRot = (controller.xRot + deltaY);
			// Clamp the X rotation to prevent the camera from going upside
			// down.
			if (controller.xRot < -90) {
				controller.xRot = -90;
			} else if (controller.xRot > 90) {
				controller.xRot = 90;
			}
			// Send the onchange event to any listener.
			if (controller.onchange != null) {
				controller.onchange(controller.xRot, controller.yRot);
			}
		}
	};
}
