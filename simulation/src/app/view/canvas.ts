import circlesFragmentShader from "./shader/circles.frag";
import circlesVertexShader from "./shader/circles.vert";

export default class Canvas {

    private readonly gl: WebGLRenderingContext;
    private readonly program: WebGLProgram;

    private readonly positionBuffer: WebGLBuffer;

    private attribs: {[attrib: string]: number} = {};
    private uniforms: {[uniform: string]: WebGLUniformLocation} = {};

    constructor(private canvas: HTMLCanvasElement) {
        const gl = this.canvas.getContext("webgl");
        if (!gl) {
            throw Error("No GL Context found");
        }
        this.gl = gl;

        const vertShader = Canvas._createShader(gl, gl.VERTEX_SHADER, circlesVertexShader);
        const fragShader = Canvas._createShader(gl, gl.FRAGMENT_SHADER, circlesFragmentShader);
        this.program = Canvas._createProgram(gl, vertShader, fragShader);

        this._fillAttribs(["position"]);
        this._fillUniforms(["resolution"]);

        this.positionBuffer = gl.createBuffer() as WebGLBuffer;
        gl.bindBuffer(gl.ARRAY_BUFFER, this.positionBuffer);
        const positions = [
            -1, -1,
            -1, 1,
            1, -1,
            1, -1,
            -1, 1,
            1, 1
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    }

    private _fillUniforms(uniformNames: string[]): void {
        for (const name of uniformNames) {
            this.uniforms[name] = this.gl.getUniformLocation(this.program, "u_" + name) as WebGLUniformLocation;
        }
    }

    private _fillAttribs(attribsNames: string[]): void {
        for (const name of attribsNames) {
            this.attribs[name] = this.gl.getAttribLocation(this.program, "a_" + name);
        }
    }

    public render(): void {
        const gl = this.gl;
        this._resize();

        gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
        gl.clearColor(0, 0, 0, 0);
        gl.clear(gl.COLOR_BUFFER_BIT);

        gl.useProgram(this.program);

        gl.enableVertexAttribArray(this.attribs["position"]);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.positionBuffer);
        gl.vertexAttribPointer(this.attribs["position"], 2, gl.FLOAT, false, 0, 0);

        this._setUniform("resolution", gl.canvas.width, gl.canvas.height);

        gl.drawArrays(gl.TRIANGLES, 0, 6);
    }

    private _setUniform(name: string, ...value: number[]): void {
        const location = this.uniforms[name];
        if (value.length == 1) {
            this.gl.uniform1f(location, value[0]);
        } else if (value.length == 2) {
            this.gl.uniform2fv(location, value);
        } else if (value.length == 3) {
            this.gl.uniform3fv(location, value);
        } else if (value.length == 4) {
            this.gl.uniform4fv(location, value);
        } else {
            throw Error("Could not set uniform: Array to large");
        }
    }

    private static _createShader(gl: WebGLRenderingContext, type: number, source: string) {
        const shader = gl.createShader(type);
        if (shader != null) {
            gl.shaderSource(shader, source);
            gl.compileShader(shader);
            const success = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
            if (success) {
                return shader;
            } else {
                let shaderName = "SHADER";
                if (type == gl.FRAGMENT_SHADER) shaderName = "FRAGMENT SHADER";
                else if (type == gl.VERTEX_SHADER) shaderName = "VERTEX SHADER";

                console.error(`${shaderName}: ${gl.getShaderInfoLog(shader)}`);
                gl.deleteShader(shader);
            }
        }
        throw Error("Could not create Shader");
    }

    private static _createProgram(gl: WebGLRenderingContext, vertexShader: WebGLShader, fragmentShader: WebGLShader): WebGLProgram {
        const program = gl.createProgram();
        if (program != null) {
            gl.attachShader(program, vertexShader);
            gl.attachShader(program, fragmentShader);
            gl.linkProgram(program);
            const success = gl.getProgramParameter(program, gl.LINK_STATUS);
            if (success) {
                return program;
            } else {
                console.error("PROGRAM: " + gl.getProgramInfoLog(program));
                gl.deleteProgram(program);
            }
        }
        throw Error("Could not create Program");
    }

    private _resize() {
        // Lookup the size the browser is displaying the canvas.
        const displayWidth  = this.canvas.clientWidth;
        const displayHeight = this.canvas.clientHeight;

        // Check if the canvas is not the same size.
        if (this.canvas.width  != displayWidth ||
            this.canvas.height != displayHeight) {

            // Make the canvas the same size
            this.canvas.width  = displayWidth;
            this.canvas.height = displayHeight;
        }
    }
}
