// Shader Background Implementation
const vertexShaderSource = `
    attribute vec2 a_position;
    
    void main() {
        gl_Position = vec4(a_position, 0.0, 1.0);
    }
`;

const fragmentShaderSource = `
    precision mediump float;
    
    uniform vec2 u_resolution;
    uniform float u_time;
    
    #define PI 3.14159265359
    
    vec2 hash22(vec2 p) {
        vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
        p3 += dot(p3, p3.yzx+19.19);
        return fract((p3.xx+p3.yz)*p3.zy);
    }
    
    float hash11(float p) {
        return length(hash22(vec2(p)));
    }
    
    float hash21(vec2 co) {
        return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
    }
    
    mat2 rmatrix(float a) {
        float c = cos(a);
        float s = sin(a);
        return mat2(c, -s, s, c);
    }
    
    float S(float x) {
        return (3.0 * x * x - 2.0 * x * x * x);
    }
    
    float noise(vec2 pos) {
        float a = hash21(vec2(floor(pos.x), floor(pos.y)));
        float b = hash21(vec2(ceil(pos.x), floor(pos.y)));
        float c = hash21(vec2(floor(pos.x), ceil(pos.y)));
        float d = hash21(vec2(ceil(pos.x), ceil(pos.y)));
    
        float s1 = S(pos.x - floor(pos.x));
        float s2 = S(pos.y - floor(pos.y));
    
        float f = a + 
                  (b - a) * s1 +
                  (c - a) * s2 +
                  (a - b - c + d) * s1 * s2;
        return (f);
    }
    
    float onoise(in vec2 pos) {
        float delta = 3.1415 / 6.0;
        float sum = 0.0;
        float power = 0.5;
    
        sum += noise(rmatrix(delta * 0.0) * pos) * power;
        power *= 0.40;
        pos *= 1.9;
        
        sum += noise(rmatrix(delta * 1.0) * pos) * power;
        power *= 0.40;
        pos *= 1.9;
        
        sum += noise(rmatrix(delta * 2.0) * pos) * power;
        
        return sum;
    }
    
    vec3 portalTexture(vec2 uv) {
        vec2 uv2 = uv;
        
        uv2 = vec2(length(uv), (atan(uv.y, uv.x) + PI) / 2.0 / PI);
        
        uv2.y = fract(uv2.y + uv2.x * 0.3 - u_time * 0.01);
        uv2.x = uv2.x * 1.0 + u_time * 0.3;
        
        vec3 color0 = vec3(0.01, 0.01, 0.01);
        vec3 color1 = vec3(0.05, 0.05, 0.05);
        vec3 color2 = vec3(0.12, 0.12, 0.12);
        vec3 color3 = vec3(0.25, 0.25, 0.25);
        
        vec2 k = vec2(10.0);
        
        float br1 = onoise(uv2 * k);
        float br2 = onoise(vec2(uv2.x, uv2.y - 1.0) * k);
        
        float br = mix(br1, br2, uv2.y);
        
        br = min(0.99, pow(br * 1.5, 2.5));
        
        float brScaled = br / 0.25;
        vec3 col;
        
        if (brScaled < 1.0) {
            col = color0;
        } else if (brScaled < 2.0) {
            col = color1;
        } else if (brScaled < 3.0) {
            col = color2;
        } else {
            col = color3;
        }
        
        return col;
    }
    
    void main() {
        vec2 uv = (gl_FragCoord.xy * 2.0 - u_resolution) / u_resolution.y;
        vec3 col = portalTexture(uv);
        gl_FragColor = vec4(col, 1.0);
    }
`;

function initShaderBackground() {
    const canvas = document.getElementById('shader-background');
    if (!canvas) {
        console.error('Shader background canvas not found');
        return;
    }

    function resizeCanvas() {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    
    resizeCanvas();
    window.addEventListener('resize', resizeCanvas);
    
    const gl = canvas.getContext('webgl');
    if (!gl) {
        console.error('WebGL not supported');
        return;
    }
    
    const vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
    const fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);
    
    const program = createProgram(gl, vertexShader, fragmentShader);
    
    const positionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    const positions = [
        -1, -1,
         1, -1,
        -1,  1,
        -1,  1,
         1, -1,
         1,  1,
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);
    
    const positionAttributeLocation = gl.getAttribLocation(program, 'a_position');
    gl.enableVertexAttribArray(positionAttributeLocation);
    gl.vertexAttribPointer(positionAttributeLocation, 2, gl.FLOAT, false, 0, 0);
    
    const resolutionUniformLocation = gl.getUniformLocation(program, 'u_resolution');
    const timeUniformLocation = gl.getUniformLocation(program, 'u_time');
    
    let startTime = Date.now();
    function render() {
        const currentTime = Date.now();
        const elapsedTime = (currentTime - startTime) / 1000;
        
        gl.viewport(0, 0, canvas.width, canvas.height);
        
        gl.clearColor(0, 0, 0, 1);
        gl.clear(gl.COLOR_BUFFER_BIT);
        
        gl.useProgram(program);
        
        gl.uniform2f(resolutionUniformLocation, canvas.width, canvas.height);
        gl.uniform1f(timeUniformLocation, elapsedTime);
        
        gl.drawArrays(gl.TRIANGLES, 0, 6);
        
        requestAnimationFrame(render);
    }
    
    render();
}

function createShader(gl, type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    
    const success = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
    if (success) {
        return shader;
    }
    
    console.error(gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
}

function createProgram(gl, vertexShader, fragmentShader) {
    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    
    const success = gl.getProgramParameter(program, gl.LINK_STATUS);
    if (success) {
        return program;
    }
    
    console.error(gl.getProgramInfoLog(program));
    gl.deleteProgram(program);
}

window.addEventListener('load', initShaderBackground); 