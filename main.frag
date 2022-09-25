#version 410
#extension all : warn

// textures are later jeez calm down
layout(pixel_center_integer) in vec4 gl_FragCoord;

out vec4 fragment_color;

uniform uint board[200];
uniform ivec2 board_dims;
uniform ivec2 board_loc;

void main() {
    uvec4 color = uvec4(0, 0, 0, 255);

    int blocks_in_row = 10;
    int block_width = board_dims.x / blocks_in_row;
    int block_height = board_dims.y / 20;

    ivec2 board_coords = ivec2(gl_FragCoord.xy) - board_loc;

    if (gl_FragCoord.x < board_loc.x || gl_FragCoord.y < board_loc.y ||
        gl_FragCoord.x > board_loc.x + board_dims.x ||
        gl_FragCoord.y > board_loc.y + board_dims.y) {
        fragment_color = vec4(0.0, 0.0, 1.0, 1.0);
        return;
    }

    switch (board[(board_coords.x / block_width) * blocks_in_row +
                  board_coords.y / block_height]) {
        // using the same skin format as tetrio and, by extension, tetrio+ so
        // that I don't have to make one up and so that I can easily import
        // textures made for them. :)
    case 0:
        color = uvec4(0, 0, 0, 255);
        break;
    case 1:
        color = uvec4(240, 0, 47, 255);
        break;
    case 2:
        color = uvec4(250, 79, 0, 255);
        break;
    case 3:
        color = uvec4(242, 156, 0, 255);
        break;
    case 4:
        color = uvec4(29, 181, 0, 255);
        break;
    case 5:
        color = uvec4(1, 157, 216, 255);
        break;
    case 6:
        color = uvec4(0, 63, 202, 255);
        break;
    case 7:
        color = uvec4(194, 9, 137, 255);
        break;
    case 8:
        color = uvec4(255, 255, 255, 255);
        break;
    case 9:
        color = uvec4(101, 101, 101, 255);
        break;
    case 10:
        color = uvec4(101, 101, 101, 255);
        break;
    case 11:
        color = uvec4(56, 56, 56, 255);
        break;
    // none for now (the x that appears when... smth)
    default:
        color = uvec4(0, 0, 0, 255);
        break;
    }
    fragment_color = vec4(color.x / 255.0, color.y / 255.0, color.z / 255.0,
                          color.w / 255.0);
}
