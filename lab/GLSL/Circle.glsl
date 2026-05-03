/* GLSL Examples
 * A collection of GLSL examples for use with the Shader toy website and other
 * GLSL environments.
 * By: 0x4248
 *
 * Circle animate.
 * Draws a circle that moves around the screen.
 */

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
  vec2 center = vec2(0.5, 0.5);

  float radius = 0.3;

  vec2 uv = fragCoord.xy / iResolution.xy;

  float distance = length(uv - center);

  if (distance < radius) {
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
  } else {
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
