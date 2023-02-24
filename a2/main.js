
var canvas;
var gl;

var program;

var near = 1;
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

var temp = [0,0,0];
var prevTime1 = 0.0;

var MS = []; // The modeling matrix stack
var TIME = 0.0; // Realtime
var dt = 0.0
var prevTime = 0.0;
var resetTimerFlag = true;
var animFlag = true;
var controller;


var currentRotation = [0,0,0];
var misslePosition = [0,4,0];
var bouncymissileVelocity = 0;
var bouncyEnergyLoss = 0.9;
var speed = -3;
var plus_min = 1;

var sphereRotation = [0,0,0];
var spherePosition = [-4,0,0];

var cubeRotation = [0,0,0];
var cubePosition = [-1,0,0];

var cylinderRotation = [0,0,0];
var cylinderPosition = [1.1,0,0];

var coneRotation = [0,0,0];
var conePosition = [3,0,0];

var blendTextures = 0;

		
// For this example we are going to store a few different textures here
var textureArray = [] ;

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
                                         "lightPosition"),flatten(lightPosition2) );
    gl.uniform1f( gl.getUniformLocation(program, 
                                        "shininess"),materialShininess );
}

function isLoaded(im) {
    if (im.complete) {
        console.log("loaded") ;
        return true ;
    }
    else {
        console.log("still not loaded!!!!") ;
        return false ;
    }
}

function loadFileTexture(tex, filename)
{
	//create and initalize a webgl texture object.
    tex.textureWebGL  = gl.createTexture();
    tex.image = new Image();
    tex.image.src = filename ;
    tex.isTextureReady = false ;
    tex.image.onload = function() { handleTextureLoaded(tex); }
}


function handleTextureLoaded(textureObj) {

    gl.bindTexture(gl.TEXTURE_2D, textureObj.textureWebGL);
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true); // otherwise the image would be flipped upsdide down
	
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, textureObj.image);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST_MIPMAP_NEAREST);
	
    gl.generateMipmap(gl.TEXTURE_2D);
	
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE); //Prevents s-coordinate wrapping (repeating)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE); //Prevents t-coordinate wrapping (repeating)
    gl.bindTexture(gl.TEXTURE_2D, null);
    console.log(textureObj.image.src) ;
    
    textureObj.isTextureReady = true ;
}

function waitForTextures(texs) {
    setTimeout(
		function() {
			   var n = 0 ;
               for ( var i = 0 ; i < texs.length ; i++ )
               {
                    console.log(texs[i].image.src) ;
                    n = n+texs[i].isTextureReady ;
               }
               wtime = (new Date()).getTime() ;
               if( n != texs.length )
               {
               		console.log(wtime + " not ready yet") ;
               		waitForTextures(texs) ;
               }
               else
               {
               		console.log("ready to render") ;
					render(0);
               }
		},
	5) ;
}

function loadImageTexture(tex, image) {
	//create and initalize a webgl texture object.
    tex.textureWebGL  = gl.createTexture();
    tex.image = new Image();

    gl.bindTexture(gl.TEXTURE_2D, tex.textureWebGL);

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, texSize, texSize, 0, gl.RGBA, gl.UNSIGNED_BYTE, image);
	
    gl.generateMipmap(gl.TEXTURE_2D);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST_MIPMAP_LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
	
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE); //Prevents s-coordinate wrapping (repeating)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE); //Prevents t-coordinate wrapping (repeating)
    gl.bindTexture(gl.TEXTURE_2D, null);

    tex.isTextureReady = true;
}

// This just calls the appropriate texture loads for this example adn puts the textures in an array
function initTexturesForExample() {
    textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"wood9.png") ;
    
    textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"stones.png") ;
    textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"stormy.png") ;
    textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"star.png") ;

    
}

// Turn texture use on and off
function toggleTextureBlending() {
    blendTextures = (blendTextures + 1) % 2
	gl.uniform1i(gl.getUniformLocation(program, "blendTextures"), blendTextures);
}


window.onload = function init() {

    canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.5, 0.5, 1.0, 1.0 );
    
    gl.enable(gl.DEPTH_TEST);
    program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );
    

    setColor(materialDiffuse);

    Cube.init(program);
    Cylinder.init(20,program);
    Cone.init(20,program);
    Sphere.init(36,program);

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
       

    initTexturesForExample() ;

    waitForTextures(textureArray);


}

function setMV() {
    modelViewMatrix = mult(viewMatrix,modelMatrix);
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix) );
    normalMatrix = inverseTranspose(modelViewMatrix);
    gl.uniformMatrix4fv(normalMatrixLoc, false, flatten(normalMatrix) );
}

function setAllMatrices() {
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix) );
    setMV();   
}

function drawCube() {
    setMV();
    Cube.draw();
}

function drawSphere() {
    setMV();
    Sphere.draw();
}

function drawCylinder() {
    setMV();
    Cylinder.draw();
}

function drawCone() {
    setMV();
    Cone.draw();
}

// Draw a Bezier patch
function drawB3(b) {
	setMV() ;
	b.draw() ;
}

function gTranslate(x,y,z) {
    modelMatrix = mult(modelMatrix,translate([x,y,z]));
}

function gRotate(theta,x,y,z) {
    modelMatrix = mult(modelMatrix,rotate(theta,[x,y,z]));
}

function gScale(sx,sy,sz) {
    modelMatrix = mult(modelMatrix,scale(sx,sy,sz));
}

function gPop() {
    modelMatrix = MS.pop();
}

function gPush() {
    MS.push(modelMatrix);
}

function render(timestamp) {


    dtt = (timestamp - prevTime1) / 1000.0;
	prevTime1 = timestamp;
	temp[0] = temp[0] + 10*dtt;
    
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    eye = vec3(1,2,10);
    MS = []; 
    modelMatrix = mat4();
    viewMatrix = lookAt(eye, at, up);
    viewMatrix = mult(viewMatrix,rotate(temp[0],[0,1,0]));
    projectionMatrix = ortho(left, right, bottom, ytop, near, far);

    setAllMatrices();
    
	if( animFlag )
    {
		dt = (timestamp - prevTime) / 1000.0;
		prevTime = timestamp;
	}
	//--------------------------------------------------------------------------------------------------------------

    //--- gPush();{    }gPop();

    gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[3].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture1"), 0);
    gl.activeTexture(gl.TEXTURE1);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[3].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture1"), 1);

    gPush();{         
        gTranslate(-3,-3,0);
        currentRotation[2] = currentRotation[2] + 30*dt;
		gRotate(currentRotation[2],0,-1,0);
        gTranslate(2,0,0);
        setColor(vec4(0.7,0.6,0.4,1.0));
        gPush();{
        gScale(1,0.5,1.5);    
        drawCube();
        }gPop();

        gPush();{     //-------------font icon
            gTranslate(0,0,1.501);  
            gPush();{    
                gScale(1.0,0.5,0.005);
                toggleTextureBlending();
                drawCube();
                toggleTextureBlending();
            }gPop();
        }gPop();


        gPush();{ //---------------------------------side armor
        gTranslate(-0.7,0,-0.8);
            for (var i = 0; i < 2; i++){
                gTranslate(0,0,0.8*i);
                gPush();{  
                    gRotate(90,1,0,0);  
                    drawCylinder();
                }gPop();
            }
            gTranslate(0,0,0.8); // some bug here 
            gRotate(90,1,0,0);  
            drawCylinder();
                }gPop();
       
            gPush();{    
                gTranslate(0.7,0,-0.8);
                for (var i = 0; i < 2; i++){
                    gTranslate(0,0,0.8*i);
                    gPush();{  
                        gRotate(90,1,0,0);  
                        drawCylinder();
                    }gPop();
                }
                gTranslate(0,0,0.8); // some bug here 
                gRotate(90,1,0,0);  
                drawCylinder();
            }gPop();

            
        gPush();{
            gRotate(30,0,-1,0);
            cylinderRotation[1] = cylinderRotation[1] + 1.2*Math.cos(0.0015*timestamp);
			gRotate(cylinderRotation[1],0,-1,0);
            gPush();{    //---------------------- fort
                gTranslate(0,0.7,0);
                gPush();{    
                    gScale(2,0.5,2);
                    gRotate(90,1,0,0);  
                    drawCylinder(); 
                 }gPop();
                 
                 gPush();{  
                    gTranslate(0,0.2,0);  
                    gScale(0.99,0.4,0.99);
                    drawSphere(); 
                 }gPop();           //----------------- cannon barrel
                 gTranslate(0,0,2);
                 gPush();{ 
                    setColor(vec4(0.3,0.2,0.2,1.0));   
                    gScale(0.3,0.4,2.1);
                    drawCylinder();
                 }gPop();
            }gPop();
            setColor(vec4(0.7,0.6,0.4,1.0));
        }gPop();
        
	
        gPush();{
            gPush();{    //------ 2 tank track
                gTranslate(-1,-0.6,0);
                gPush();{    
                   gScale(0.47,0.02,1.4);
                    drawCube();
                }gPop();
                gTranslate(0,-0.83,0);
                gPush();{    
                    gScale(0.47,0.02,1.4);
                     drawCube();
                 }gPop();
            }gPop();
            gTranslate(-1,-1,-1.3); //-------right tank wheels
            for (var i = 0; i < 4; i++){
            gPush();{    
                gTranslate(0,0,0.9*i);
                gScale(0.9,0.9,0.9); 
                gPush();{ 
                    gTranslate(-0.1,0,0);   
                    gScale(0.5,0.5,0.5); 
                    drawSphere();
                }gPop();
                gRotate(90,0,1,0);
                drawCylinder(); 
             }gPop();
            }
        }gPop();
        //-----------------------
        gPush();{
            gPush();{    //------ 2 tank track
                gTranslate(1,-0.6,0);
                gPush();{    
                   gScale(0.47,0.02,1.4);
                    drawCube();
                }gPop();
                gTranslate(0,-0.83,0);
                gPush();{    
                    gScale(0.47,0.02,1.4);
                     drawCube();
                 }gPop();
            }gPop();
            gTranslate(1,-1,-1.3); //-------right tank wheels
            for (var i = 0; i < 4; i++){
            gPush();{    
                gTranslate(0,0,0.9*i);
                gScale(0.9,0.9,0.9); 
                gPush();{ 
                    gTranslate(-0.1,0,0);   
                    gScale(0.5,0.5,0.5); 
                    drawSphere();
                }gPop();
                gRotate(90,0,1,0);
                drawCylinder(); 
             }gPop();
            }
        }gPop();

    }gPop();

//-------------------------------------gPush();{    }gPop(); 地鼠

gPush();{  
    //gRotate(10,0,0,-1);
    gTranslate(5,-4.9,0); 
    coneRotation [0] += 0.02*Math.cos(0.002*timestamp + 1);    
    coneRotation [1] += 10*dt;    
    gRotate(coneRotation[1],0,-1,0);               
    gTranslate(0,coneRotation[0],0);
    gPush();{    
        gRotate(90,1,0,0);
        drawCylinder();
    }gPop(); 
    gTranslate(0,0.49,0);
    gPush();{   
        gScale(0.5,0.5,0.5);
        drawSphere();
    }gPop();
    gPush();{   //nose 
        gTranslate(0,-0.2,0.5);
        gPush();{    
            gScale(0.2,0.1,0.01)
            setColor(vec4(0.9,0.5,0.8,1));
            drawSphere();
        }gPop();
    }gPop();
    for (var i = 1; i < 3; i++){ //eyes
    gPush();{   
        if (i == 1){
            gTranslate(0.14,0.1,0.5);
        }else{
            gTranslate(-0.14,0.1,0.5);
        }
        gPush();{    
            gScale(0.07,0.15,0.01)
            setColor(vec4(0.0,0.0,0.0,1));
            drawCube();
        }gPop();
        gPush();{    
            gTranslate(0.0,0.1,0.01);
            gScale(0.07,0.05,0.01);
            setColor(vec4(1,1,1,1.0));    
            drawCube();
        }gPop(); 
    }gPop();
    }
    
    setColor(vec4(0.7,0.6,0.4,1.0));    
}gPop();



// ----------------------------------------------------导弹身 

    gPush();{  
        gTranslate(20,4.9,0);
        bouncymissileVelocity += speed*dt;  
		misslePosition[0] += bouncymissileVelocity*dt;  
		if (misslePosition[0] < -30)
		{
			bouncymissileVelocity = - 0.6 *bouncymissileVelocity; 
			misslePosition[0] = 30; // reset missile postiosin
		}
		gTranslate(misslePosition[0],0,0);
        gRotate(90,0,0,1);
        for (var i = 0; i < 2; i++){
            gTranslate(0,i*0.5*-plus_min,i*3*plus_min);
            plus_min *= -1;
        gPush();{
            gRotate(90,1,0,0); 
            gScale(1,1,1.5);   
            drawCylinder();
        }gPop();
        //-------------------wings
        gPush();{  
            gRotate(90,-1,0,0); 
            gScale(1.5,0.1,1);
            drawCone();
            gPush();{    
                gTranslate(0,5,0);
                gScale(0.05,5,1);
                drawCone();
            }gPop();
            gPush();{    
                gTranslate(0,-5,0);
                gScale(0.05,5,1);
                drawCone();
            }gPop();
        }gPop();
    //---------------- 导弹头
        gPush();{      
            gTranslate(0,0.7,0);
            gPush();{   
            gRotate(90,1,0,0);
            gScale(0.5,0.5,0.5);
            setColor(vec4(0.7,0.0,0.0,1.0));
            drawSphere();
            setColor(vec4(0.7,0.6,0.4,1.0));
            }gPop();
        }gPop();
    //----------------------导弹尾部
        gPush();{    
            gTranslate(0,-0.5,0);
            gRotate(90,-1,0,0);
            gPush();{    
                drawCylinder();
                gScale(0.7,0.7,2);
                drawCone();
            }gPop();         
        }gPop();
    }
    }gPop();

    //setColor(vec4(0.7,0.6,0.4,1.0));


//--------------------------------------------------------------------------------------------------------------------- 沙漠地面
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[0].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture1"), 0);
	
	gl.activeTexture(gl.TEXTURE1);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[1].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture2"), 1);
	setColor(vec4(0.7,0.7,0.7,0.15));
	gPush();
	{
		gTranslate(0,-5,0);
        gRotate(90,-1,0,0);
		gPush();
		{
			gScale(30,30,1);
			toggleTextureBlending();
			drawCone();
			toggleTextureBlending();
		}
		gPop();
		
	}
	gPop();
    //---------------------------------------------------------------------------天空
    gl.bindTexture(gl.TEXTURE_2D, textureArray[2].textureWebGL);
    gl.uniform1i(gl.getUniformLocation(program, "texture1"), 1);


    setColor(vec4(0,0,0,1));
	gPush();
		//gTranslate(cylinderPosition[0],cylinderPosition[1],cylinderPosition[2]);
		gPush();
		{
			gRotate(90,1,0,0);
            gPush();{    
                toggleTextureBlending();
                gScale(35,27,20);
                drawCylinder();
                toggleTextureBlending();
            }gPop();
		}
		gPop();
	gPop();	   

    
    if( animFlag )
        window.requestAnimFrame(render);
}

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

