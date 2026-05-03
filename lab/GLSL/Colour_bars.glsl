/* GLSL Examples
 * A collection of GLSL examples for use with the Shader toy website and other
 * GLSL environments.
 * By: 0x4248
 *
 * Color Bars
 * Basic colour bars example.
 */

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
  vec2 uv = fragCoord.xy / iResolution.xy;

  float barWidth = 1.0 / 8.0;

  int barIndex = int(uv.x / barWidth);

  vec3 colors[8];
  colors[0] = vec3(1.0, 0.0, 0.0); // Red
  colors[1] = vec3(1.0, 0.5, 0.0); // Orange
  colors[2] = vec3(1.0, 1.0, 0.0); // Yellow
  colors[3] = vec3(0.0, 1.0, 0.0); // Green
  colors[4] = vec3(0.0, 0.0, 1.0); // Blue
  colors[5] = vec3(0.5, 0.0, 1.0); // Purple
  colors[6] = vec3(1.0, 0.0, 1.0); // Magenta
  colors[7] = vec3(1.0, 1.0, 1.0); // White

  vec3 color = colors[barIndex];

  fragColor = vec4(color, 1.0);
}
