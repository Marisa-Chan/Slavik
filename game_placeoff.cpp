#include "game.h"

namespace Game
{

const Common::Point Engine::PlaceDOff[2][8][12] = 
{{
{{0, -1},   {-1, -1},  {-2, 0},   {-1, -3},   {-2, -2},   {-2, -1}, 
 {-2, -3},  {-3, -2},  {-2, -5},  {-3, -4},   {-3, -3},   {-3, -1}},
{{-1, -2},  {0, -3},   {0, -5},   {-1, -4},   {0, -6},    {-1, -6},
 {-1, -5},  {-2, -4},  {0, -7},   {-1, -7},   {-2, -6},   {-2, -7}},
{{1, -2},   {1, -3},   {1, -5},   {1, -4},    {0, -6},    {1, -6},
 {2, -5},   {2, -4},   {1, -7},   {2, -7},    {2, -6},    {3, -7}},
{{1, -1},   {2, -1},   {2, 0},    {2, -3},    {2, -2},    {3, -1},
 {3, -3},   {3, -2},   {3, -5},   {3, -4},    {4, -3},    {4, -1}},
{{1, 1},    {2, 1},    {2, 0},    {2, 3},     {2, 2},     {3, 1},
 {3, 3},    {3, 2},    {3, 5},    {3, 4},     {4, 3},     {4, 1}},
{{1, 2},    {1, 3},    {1, 5},    {1, 4},     {0, 6},     {1, 6},
 {2, 5},    {2, 4},    {1, 7},    {2, 7},     {2, 6},     {3, 7}},
{{-1, 2},   {0, 3},    {0, 5},    {-1, 4},    {0, 6},     {-1, 6},
 {-1, 5},   {-2, 4},   {0, 7},    {-1, 7},    {-2, 6},    {-2, 7}},
{{0, 1},    {-1, 1},   {-2, 0},   {-1, 3},    {-2, 2},    {-2, 1},
 {-2, 3},   {-3, 2},   {-2, 5},   {-3, 4},    {-3, 3},    {-3, 1}}
},{
{{-1, -1},  {-2, -1},  {-2, 0},   {-2, -3},   {-2, -2},   {-3, -1},
 {-3, -3},  {-3, -2},  {-3, -5},  {-3, -4},   {-4, -3},   {-4, -1}},
{{-1, -2},  {-1, -3},  {-1, -5},  {-1, -4},   {0, -6},    {-1, -6},
 {-2, -5},  {-2, -4},  {-1, -7},  {-2, -7},   {-2, -6},   {-3, -7}},
{{1, -2},   {0, -3},   {0, -5},   {1, -4},    {0, -6},    {1, -6},
 {1, -5},   {2, -4},   {0, -7},   {1, -7},    {2, -6},    {2, -7}},
{{0, -1},   {1, -1},   {2, 0},    {1, -3},    {2, -2},    {2, -1},
 {2, -3},   {3, -2},   {2, -5},   {3, -4},    {3, -3},    {3, -1}},
{{0, 1},    {1, 1},    {2, 0},    {1, 3},     {2, 2},     {2, 1},
 {2, 3},    {3, 2},    {2, 5},    {3, 4},     {2, 3},     {3, 1}},
{{1, 2},    {0, 3},    {0, 5},    {1, 4},     {0, 6},     {1, 6},
 {1, 5},    {2, 4},    {0, 7},    {1, 7},     {2, 6},     {2, 7}},
{{-1, 2},   {-1, 3},   {-1, 5},   {-1, 4},    {0, 6},     {-1, 6},
 {-2, 5},   {-2, 4},   {-1, 7},   {-2, 7},    {-2, 6},    {-3, 7}},
{{-1, 1},   {-2, 1},   {-2, 0},   {-2, 3},    {-2, 2},    {-3, 1},
 {-3, 3},   {-3, 2},   {-3, 5},   {-3, 4},    {-4, 3},    {-4, 1}}}};

}