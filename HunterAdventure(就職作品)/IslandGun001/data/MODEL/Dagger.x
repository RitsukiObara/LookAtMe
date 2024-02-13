xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 276;
 -2.21461;2.21461;5.96136;,
 -1.93585;1.93585;11.92272;,
 0.00000;2.73771;11.92272;,
 0.00000;3.13194;5.96136;,
 0.00000;3.30900;0.00000;,
 -2.33982;2.33982;0.00000;,
 -3.30900;0.00000;0.00000;,
 -3.13194;-0.00000;5.96136;,
 -2.73771;-0.00000;11.92272;,
 0.00000;3.13194;5.96136;,
 0.00000;2.73771;11.92272;,
 1.93585;1.93585;11.92272;,
 2.21461;2.21461;5.96136;,
 2.33982;2.33982;0.00000;,
 0.00000;3.30900;0.00000;,
 -2.33982;-2.33982;-0.00000;,
 -2.21461;-2.21461;5.96136;,
 -1.93585;-1.93585;11.92272;,
 2.73771;-0.00000;11.92272;,
 3.13194;-0.00000;5.96136;,
 3.30900;-0.00000;-0.00000;,
 -0.00000;-3.30900;-0.00000;,
 -0.00000;-3.13194;5.96136;,
 -0.00000;-2.73771;11.92272;,
 1.93585;-1.93585;11.92272;,
 2.21461;-2.21461;5.96136;,
 2.33982;-2.33982;-0.00000;,
 -2.21461;2.21461;-5.96136;,
 0.00000;3.13194;-5.96136;,
 0.00000;2.73771;-11.92272;,
 -1.93600;1.93600;-11.92240;,
 -2.73771;0.00000;-11.92272;,
 -3.13194;0.00000;-5.96136;,
 0.00000;3.13194;-5.96136;,
 2.21461;2.21461;-5.96136;,
 1.93585;1.93585;-11.92272;,
 0.00000;2.73771;-11.92272;,
 -1.93585;-1.93585;-11.92272;,
 -2.21461;-2.21461;-5.96136;,
 3.13194;-0.00000;-5.96136;,
 2.73771;0.00000;-11.92272;,
 -0.00000;-2.73771;-11.92272;,
 -0.00000;-3.13194;-5.96136;,
 2.21461;-2.21461;-5.96136;,
 1.93585;-1.93585;-11.92272;,
 0.00000;-0.00000;25.35633;,
 1.88263;1.88263;24.82673;,
 0.00000;1.88240;24.82640;,
 2.66244;-0.00000;24.82673;,
 -1.88263;1.88263;24.82673;,
 -2.66244;-0.00000;24.82673;,
 3.47864;3.47864;23.31859;,
 0.00000;3.47840;23.31840;,
 4.91955;-0.00000;23.31859;,
 -3.47864;3.47864;23.31859;,
 -4.91955;-0.00000;23.31859;,
 6.42770;-0.00000;21.06148;,
 4.54507;3.47867;21.06148;,
 -4.54507;3.47867;21.06148;,
 -6.42770;-0.00000;21.06148;,
 6.95729;-0.00000;18.39904;,
 4.91955;3.47875;18.39904;,
 -4.91955;3.47875;18.39904;,
 -6.95729;0.00000;18.39904;,
 6.42770;-0.00000;15.73660;,
 4.54507;3.47867;15.73669;,
 -6.95729;0.00000;18.39904;,
 -4.91955;3.47875;18.39904;,
 -4.54507;3.47867;15.73669;,
 -6.42770;0.00000;15.73660;,
 4.91955;-0.00000;13.47949;,
 3.47864;3.47864;13.47949;,
 -3.47864;3.47864;13.47949;,
 -4.91955;0.00000;13.47949;,
 0.00000;3.47840;13.47931;,
 1.93585;1.93585;11.92272;,
 0.00000;2.73771;11.92272;,
 2.73771;-0.00000;11.92272;,
 -1.93585;1.93585;11.92272;,
 -2.73771;-0.00000;11.92272;,
 1.32602;4.25952;18.39927;,
 0.00000;4.25952;18.39927;,
 0.93764;4.25952;19.33691;,
 0.93764;4.25952;17.46164;,
 0.00000;4.25967;17.06804;,
 -0.93764;4.25952;17.46164;,
 0.00000;4.25952;18.39927;,
 -1.32602;4.25952;18.39927;,
 -0.93764;4.25952;17.46164;,
 -0.93764;4.25952;19.33691;,
 0.00000;4.25967;19.73044;,
 2.25033;3.47810;20.64962;,
 3.18245;3.47810;18.39929;,
 2.25033;3.47810;16.14896;,
 0.00000;3.47840;15.20432;,
 -2.25033;3.47810;16.14896;,
 -3.18245;3.47810;18.39929;,
 -1.32602;4.25952;18.39927;,
 -2.25033;3.47810;20.64962;,
 -0.93764;4.25952;19.33691;,
 -2.25033;3.47810;20.64962;,
 0.00000;3.47840;21.59408;,
 2.86449;3.47837;14.81423;,
 0.00000;3.47840;14.34181;,
 3.86376;3.47838;17.06799;,
 -2.86449;3.47837;14.81423;,
 -3.86376;3.47838;17.06799;,
 -2.86449;3.47837;21.98411;,
 -3.18245;3.47810;18.39929;,
 -3.86376;3.47838;19.73039;,
 0.00000;3.47840;22.45624;,
 2.86449;3.47837;21.98411;,
 3.86376;3.47838;19.73039;,
 0.00000;-1.88240;24.82640;,
 1.88263;-1.88263;24.82673;,
 -1.88263;-1.88263;24.82673;,
 0.00000;-3.47840;23.31840;,
 3.47864;-3.47864;23.31859;,
 -3.47864;-3.47864;23.31859;,
 4.54507;-3.47867;21.06148;,
 -4.54507;-3.47867;21.06148;,
 4.91955;-3.47875;18.39904;,
 -4.91955;-3.47875;18.39904;,
 -4.54507;-3.47867;21.06148;,
 -6.42770;-0.00000;21.06148;,
 4.54507;-3.47867;15.73669;,
 -4.54507;-3.47867;15.73669;,
 3.47864;-3.47864;13.47949;,
 -3.47864;-3.47864;13.47949;,
 -0.00000;-2.73771;11.92272;,
 1.93585;-1.93585;11.92272;,
 0.00000;-3.47840;13.47931;,
 -1.93585;-1.93585;11.92272;,
 0.93764;-4.25952;19.33691;,
 0.00000;-4.25952;18.39927;,
 1.32602;-4.25952;18.39927;,
 0.93764;-4.25952;17.46164;,
 0.00000;-4.25967;17.06804;,
 -0.93764;-4.25952;17.46164;,
 0.00000;-4.25952;18.39927;,
 -1.32602;-4.25952;18.39927;,
 -1.32602;-4.25952;18.39927;,
 -0.93764;-4.25952;19.33691;,
 0.00000;-4.25967;19.73044;,
 3.18245;-3.47810;18.39929;,
 2.25033;-3.47810;20.64962;,
 2.25033;-3.47810;16.14896;,
 0.00000;-3.47840;15.20432;,
 -2.25033;-3.47810;16.14896;,
 -3.18245;-3.47810;18.39929;,
 -2.25033;-3.47810;20.64962;,
 -3.18245;-3.47810;18.39929;,
 0.00000;-3.47840;21.59408;,
 0.00000;-3.47840;14.34181;,
 2.86449;-3.47837;14.81423;,
 3.86376;-3.47838;17.06799;,
 -2.86449;-3.47837;14.81423;,
 -3.86376;-3.47838;17.06799;,
 -3.86376;-3.47838;19.73039;,
 -2.86449;-3.47837;21.98411;,
 0.00000;-3.47840;22.45624;,
 -4.91955;-3.47875;18.39904;,
 -3.86376;-3.47838;19.73039;,
 2.86449;-3.47837;21.98411;,
 3.86376;-3.47838;19.73039;,
 0.00000;4.33760;-17.06458;,
 -3.06640;3.06640;-17.06458;,
 -3.06640;3.06640;-11.92240;,
 0.00000;4.33760;-11.92240;,
 0.00000;-4.33760;-17.06458;,
 0.00000;-4.33760;-11.92240;,
 -3.06640;-3.06640;-11.92240;,
 -3.06640;-3.06640;-17.06458;,
 -15.53017;0.00000;-17.06444;,
 -11.92270;-0.68426;-17.06444;,
 -11.92270;-0.68426;-11.92226;,
 -13.70924;0.00000;-11.92240;,
 -11.92270;0.68426;-11.92226;,
 -11.92270;0.68426;-17.06444;,
 -4.77521;0.70668;-11.92239;,
 -11.92270;0.68426;-11.92226;,
 -13.70924;0.00000;-11.92240;,
 -4.33760;0.00000;-11.92240;,
 -4.77521;0.70668;-11.92239;,
 -3.06640;3.06640;-11.92240;,
 -3.06640;3.06640;-17.06458;,
 -4.77521;0.70668;-17.06457;,
 -11.92270;0.68426;-17.06444;,
 -11.92270;-0.68426;-11.92226;,
 -4.77521;-0.70668;-11.92239;,
 -4.77521;0.70668;-17.06457;,
 -4.33760;0.00000;-17.06458;,
 -15.53017;0.00000;-17.06444;,
 -11.92270;0.68426;-17.06444;,
 -4.77521;-0.70668;-11.92239;,
 -11.92270;-0.68426;-17.06444;,
 -4.77521;-0.70668;-17.06457;,
 -3.06640;-3.06640;-17.06458;,
 -3.06640;-3.06640;-11.92240;,
 -4.77521;-0.70668;-17.06457;,
 -11.92270;-0.68426;-17.06444;,
 -3.06640;3.06640;-11.92240;,
 -1.93600;1.93600;-11.92240;,
 0.00000;2.73771;-11.92272;,
 -4.33760;0.00000;-11.92240;,
 -2.73771;0.00000;-11.92272;,
 -1.93585;-1.93585;-11.92272;,
 -3.06640;-3.06640;-11.92240;,
 -0.00000;-2.73771;-11.92272;,
 3.06640;3.06640;-11.92240;,
 3.06640;3.06640;-17.06458;,
 3.06640;-3.06640;-17.06458;,
 3.06640;-3.06640;-11.92240;,
 13.70924;0.00000;-11.92240;,
 11.92270;-0.68426;-11.92226;,
 11.92270;-0.68426;-17.06444;,
 15.53017;0.00000;-17.06444;,
 11.92270;0.68426;-17.06444;,
 11.92270;0.68426;-11.92226;,
 4.33760;0.00000;-11.92240;,
 13.70924;0.00000;-11.92240;,
 11.92270;0.68426;-11.92226;,
 4.77521;0.70668;-11.92239;,
 4.77521;0.70668;-17.06457;,
 3.06640;3.06640;-17.06458;,
 3.06640;3.06640;-11.92240;,
 4.77521;0.70668;-11.92239;,
 11.92270;0.68426;-17.06444;,
 4.77521;-0.70668;-11.92239;,
 11.92270;-0.68426;-11.92226;,
 11.92270;0.68426;-17.06444;,
 15.53017;0.00000;-17.06444;,
 4.33760;0.00000;-17.06458;,
 4.77521;0.70668;-17.06457;,
 4.77521;-0.70668;-17.06457;,
 11.92270;-0.68426;-17.06444;,
 4.77521;-0.70668;-11.92239;,
 3.06640;-3.06640;-11.92240;,
 3.06640;-3.06640;-17.06458;,
 11.92270;-0.68426;-17.06444;,
 4.77521;-0.70668;-17.06457;,
 1.93585;1.93585;-11.92272;,
 3.06640;3.06640;-11.92240;,
 2.73771;0.00000;-11.92272;,
 4.33760;0.00000;-11.92240;,
 3.06640;-3.06640;-11.92240;,
 1.93585;-1.93585;-11.92272;,
 -4.33760;0.00000;-11.92240;,
 -4.33760;0.00000;-17.06458;,
 -3.06640;3.06640;-17.06458;,
 -3.06640;-3.06640;-17.06458;,
 4.33760;0.00000;-17.06458;,
 3.06640;3.06640;-17.06458;,
 3.06640;-3.06640;-17.06458;,
 4.33760;0.00000;-11.92240;,
 -4.33760;0.00000;-17.06458;,
 -2.20891;0.69470;-17.14480;,
 4.33760;0.00000;-17.06458;,
 2.20960;0.69470;-17.14480;,
 -2.20891;-0.69445;-17.14480;,
 2.20960;-0.69445;-17.14480;,
 2.20925;0.69453;-49.75723;,
 2.20960;0.69470;-17.14480;,
 4.33760;0.00000;-17.06480;,
 4.33760;0.00000;-49.75738;,
 2.20960;-0.69445;-17.14480;,
 2.20925;-0.69462;-49.75723;,
 -2.20891;0.69470;-17.14480;,
 -2.20925;0.69453;-49.75723;,
 -4.33760;0.00000;-49.75738;,
 -4.33760;0.00000;-17.06480;,
 -2.20925;-0.69462;-49.75723;,
 -2.20891;-0.69445;-17.14480;,
 0.00000;-0.00007;-69.72868;,
 0.00000;-0.00007;-69.72868;,
 0.00000;-0.00007;-69.72868;;
 
 214;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;0,5,6,7;,
 4;0,7,8,1;,
 4;9,10,11,12;,
 4;9,12,13,14;,
 4;7,6,15,16;,
 4;7,16,17,8;,
 4;12,11,18,19;,
 4;12,19,20,13;,
 4;16,15,21,22;,
 4;16,22,23,17;,
 4;19,18,24,25;,
 4;19,25,26,20;,
 4;22,21,26,25;,
 4;22,25,24,23;,
 4;27,5,4,28;,
 4;27,28,29,30;,
 4;27,30,31,32;,
 4;27,32,6,5;,
 4;33,14,13,34;,
 4;33,34,35,36;,
 4;32,31,37,38;,
 4;32,38,15,6;,
 4;34,13,20,39;,
 4;34,39,40,35;,
 4;38,37,41,42;,
 4;38,42,21,15;,
 4;39,20,26,43;,
 4;39,43,44,40;,
 4;42,41,44,43;,
 4;42,43,26,21;,
 3;45,46,47;,
 3;45,48,46;,
 3;45,49,50;,
 3;45,47,49;,
 4;47,46,51,52;,
 4;46,48,53,51;,
 4;50,49,54,55;,
 4;49,47,52,54;,
 4;51,53,56,57;,
 4;55,54,58,59;,
 4;57,56,60,61;,
 4;59,58,62,63;,
 4;61,60,64,65;,
 4;66,67,68,69;,
 4;65,64,70,71;,
 4;69,68,72,73;,
 4;74,71,75,76;,
 4;71,70,77,75;,
 4;73,72,78,79;,
 4;72,74,76,78;,
 3;80,81,82;,
 3;83,81,80;,
 3;84,81,83;,
 3;85,86,84;,
 3;87,81,88;,
 3;89,81,87;,
 3;90,82,81;,
 3;90,81,89;,
 4;80,82,91,92;,
 4;83,80,92,93;,
 4;93,94,84,83;,
 4;85,84,94,95;,
 4;95,96,97,85;,
 4;96,98,99,97;,
 4;90,89,100,101;,
 4;90,101,91,82;,
 4;102,71,74,103;,
 4;102,103,94,93;,
 4;102,93,92,104;,
 4;102,104,65,71;,
 4;103,74,72,105;,
 4;103,105,95,94;,
 3;104,61,65;,
 3;104,92,61;,
 4;105,72,68,106;,
 4;105,106,96,95;,
 3;106,67,96;,
 3;106,68,67;,
 4;107,100,108,109;,
 4;107,109,58,54;,
 4;107,54,52,110;,
 4;107,110,101,100;,
 3;109,62,58;,
 3;109,108,62;,
 4;110,52,51,111;,
 4;110,111,91,101;,
 4;111,51,57,112;,
 4;111,112,92,91;,
 3;112,61,92;,
 3;112,57,61;,
 3;113,114,45;,
 3;114,48,45;,
 3;50,115,45;,
 3;115,113,45;,
 4;116,117,114,113;,
 4;117,53,48,114;,
 4;55,118,115,50;,
 4;118,116,113,115;,
 4;119,56,53,117;,
 4;59,120,118,55;,
 4;121,60,56,119;,
 4;66,122,123,124;,
 4;125,64,60,121;,
 4;69,126,122,66;,
 4;127,70,64,125;,
 4;73,128,126,69;,
 4;129,130,127,131;,
 4;130,77,70,127;,
 4;79,132,128,73;,
 4;132,129,131,128;,
 3;133,134,135;,
 3;135,134,136;,
 3;136,134,137;,
 3;137,134,138;,
 3;138,139,140;,
 3;141,134,142;,
 3;134,133,143;,
 3;142,134,143;,
 4;144,145,133,135;,
 4;146,144,135,136;,
 4;136,137,147,146;,
 4;148,147,137,138;,
 4;138,140,149,148;,
 4;141,142,150,151;,
 4;152,150,142,143;,
 4;133,145,152,143;,
 4;153,131,127,154;,
 4;146,147,153,154;,
 4;155,144,146,154;,
 4;127,125,155,154;,
 4;156,128,131,153;,
 4;147,148,156,153;,
 3;125,121,155;,
 3;121,144,155;,
 4;157,126,128,156;,
 4;148,149,157,156;,
 3;149,122,157;,
 3;122,126,157;,
 4;158,151,150,159;,
 4;118,120,158,159;,
 4;160,116,118,159;,
 4;150,152,160,159;,
 3;120,161,158;,
 3;122,149,162;,
 4;163,117,116,160;,
 4;152,145,163,160;,
 4;164,119,117,163;,
 4;145,144,164,163;,
 3;144,121,164;,
 3;121,119,164;,
 4;165,166,167,168;,
 4;169,170,171,172;,
 4;173,174,175,176;,
 4;173,176,177,178;,
 4;179,180,181,182;,
 4;183,184,185,186;,
 4;183,186,187,177;,
 4;182,181,188,189;,
 4;190,191,192,193;,
 4;194,175,195,196;,
 4;194,196,197,198;,
 4;191,199,200,192;,
 4;201,202,203,168;,
 4;202,201,204,205;,
 4;206,205,204,207;,
 4;206,207,170,208;,
 4;168,209,210,165;,
 4;211,212,170,169;,
 4;213,214,215,216;,
 4;217,218,213,216;,
 4;219,220,221,222;,
 4;223,224,225,226;,
 4;218,227,223,226;,
 4;228,229,220,219;,
 4;230,231,232,233;,
 4;234,235,214,236;,
 4;237,238,234,236;,
 4;231,239,240,232;,
 4;168,203,241,242;,
 4;243,244,242,241;,
 4;245,244,243,246;,
 4;208,170,245,246;,
 3;247,201,179;,
 3;247,189,207;,
 3;248,190,249;,
 3;248,250,199;,
 3;251,252,233;,
 3;251,240,253;,
 3;254,222,242;,
 3;254,245,228;,
 3;255,249,256;,
 3;257,258,252;,
 3;165,256,249;,
 3;165,252,258;,
 3;258,256,165;,
 3;191,259,250;,
 3;259,169,250;,
 3;260,257,253;,
 3;253,169,260;,
 3;259,260,169;,
 4;261,262,263,264;,
 4;264,263,265,266;,
 4;267,268,269,270;,
 4;270,269,271,272;,
 4;267,262,261,268;,
 4;271,266,265,272;,
 3;268,273,269;,
 3;264,274,261;,
 3;269,273,271;,
 3;266,274,264;,
 3;275,268,261;,
 3;275,266,271;;
 
 MeshMaterialList {
  3;
  214;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\GoldGloss.png";
   }
  }
  Material {
   0.357647;0.232157;0.062745;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\SilverGloss.png";
   }
  }
 }
 MeshNormals {
  235;
  -0.000000;0.959068;-0.283177;,
  0.897537;-0.000000;-0.440939;,
  0.612079;-0.698799;-0.370188;,
  -0.897537;0.000000;-0.440939;,
  -0.612079;0.698799;-0.370188;,
  0.000021;0.997821;-0.065978;,
  0.997820;-0.000000;-0.065987;,
  -0.000000;-0.997821;-0.065987;,
  -0.705566;-0.705566;-0.065987;,
  -0.997821;-0.000021;-0.065979;,
  -0.705566;0.705566;-0.065972;,
  -0.707107;0.707107;0.000000;,
  -0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.707107;0.707107;0.000000;,
  -0.707107;-0.707107;-0.000000;,
  1.000000;-0.000000;0.000000;,
  -0.000000;-1.000000;0.000000;,
  0.707107;-0.707107;-0.000000;,
  -0.706297;0.706297;0.047848;,
  -0.000000;0.998855;0.047848;,
  -0.998855;-0.000000;0.047848;,
  0.706297;0.706297;0.047848;,
  -0.706297;-0.706297;0.047848;,
  0.998855;-0.000000;0.047848;,
  -0.000000;-0.998855;0.047848;,
  0.706297;-0.706297;0.047848;,
  -0.706297;0.706297;-0.047841;,
  0.000010;0.998855;-0.047844;,
  -0.998855;-0.000010;-0.047844;,
  0.706297;0.706297;-0.047848;,
  -0.706297;-0.706297;-0.047848;,
  0.998855;-0.000000;-0.047848;,
  -0.000000;-0.998855;-0.047848;,
  0.706297;-0.706297;-0.047848;,
  0.612079;0.698799;-0.370188;,
  -0.000000;-0.000000;1.000000;,
  -0.000000;0.493208;0.869912;,
  0.195004;0.334758;0.921906;,
  0.380610;-0.000000;0.924736;,
  -0.380610;-0.000000;0.924736;,
  -0.195003;0.334748;0.921910;,
  -0.000000;0.918398;0.395658;,
  0.493151;0.473625;0.729713;,
  0.728758;-0.000000;0.684771;,
  -0.728758;-0.000000;0.684771;,
  -0.493150;0.473611;0.729723;,
  0.845000;0.439589;0.304526;,
  0.940772;-0.000000;0.339041;,
  -0.940772;0.000000;0.339040;,
  -0.845000;0.439590;0.304526;,
  0.871318;0.490719;-0.000001;,
  1.000000;-0.000000;-0.000001;,
  -1.000000;0.000000;-0.000001;,
  -0.871318;0.490719;-0.000001;,
  0.844999;0.439593;-0.304523;,
  0.940773;-0.000000;-0.339038;,
  -0.940773;0.000000;-0.339038;,
  -0.844999;0.439593;-0.304523;,
  -0.000000;0.953442;-0.301575;,
  0.371765;0.819773;-0.435618;,
  0.739644;-0.000000;-0.672998;,
  -0.739644;0.000000;-0.672998;,
  -0.371765;0.819773;-0.435618;,
  0.139394;0.980488;0.138607;,
  -0.000000;0.980637;0.195835;,
  -0.139394;0.980488;0.138607;,
  0.139401;0.980488;-0.138605;,
  0.000001;0.980638;-0.195830;,
  -0.139400;0.980488;-0.138603;,
  -0.130302;0.991474;-0.000000;,
  0.130302;0.991474;-0.000000;,
  0.000000;1.000000;-0.000000;,
  0.139473;0.980474;0.138629;,
  0.196630;0.980478;-0.000000;,
  0.139481;0.980473;-0.138627;,
  0.000001;0.980638;-0.195829;,
  -0.139480;0.980474;-0.138625;,
  -0.196630;0.980478;-0.000000;,
  -0.139473;0.980474;0.138629;,
  0.000000;0.980637;0.195834;,
  -0.000120;1.000000;0.000100;,
  -0.000000;1.000000;0.000104;,
  -0.000301;1.000000;0.000060;,
  0.000120;1.000000;0.000100;,
  0.000300;1.000000;0.000060;,
  0.000120;1.000000;-0.000100;,
  0.000300;1.000000;-0.000060;,
  -0.000000;1.000000;-0.000104;,
  -0.000120;1.000000;-0.000100;,
  -0.000301;1.000000;-0.000060;,
  -0.000000;-0.959068;-0.283177;,
  -0.612079;-0.698799;-0.370188;,
  -0.000000;-0.493208;0.869912;,
  0.195004;-0.334758;0.921906;,
  -0.195003;-0.334748;0.921910;,
  -0.000000;-0.918398;0.395658;,
  0.493151;-0.473625;0.729713;,
  -0.493150;-0.473611;0.729723;,
  0.845000;-0.439590;0.304526;,
  -0.845000;-0.439589;0.304526;,
  0.871318;-0.490719;-0.000001;,
  -0.871318;-0.490719;-0.000001;,
  0.844999;-0.439593;-0.304523;,
  -0.844999;-0.439593;-0.304523;,
  -0.000000;-0.953443;-0.301575;,
  0.371765;-0.819773;-0.435618;,
  -0.371766;-0.819773;-0.435618;,
  0.139394;-0.980488;0.138607;,
  -0.000000;-0.980637;0.195835;,
  -0.139394;-0.980488;0.138607;,
  0.139401;-0.980488;-0.138605;,
  0.000001;-0.980638;-0.195830;,
  -0.139400;-0.980488;-0.138603;,
  -0.130302;-0.991474;-0.000000;,
  0.130302;-0.991474;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.139473;-0.980474;0.138629;,
  0.196630;-0.980478;-0.000000;,
  0.139481;-0.980473;-0.138627;,
  0.000001;-0.980638;-0.195829;,
  -0.139480;-0.980474;-0.138625;,
  -0.196630;-0.980478;-0.000000;,
  -0.139473;-0.980474;0.138629;,
  0.000000;-0.980637;0.195834;,
  -0.000120;-1.000000;0.000100;,
  -0.000000;-1.000000;0.000104;,
  -0.000301;-1.000000;0.000060;,
  0.000120;-1.000000;0.000100;,
  0.000300;-1.000000;0.000060;,
  0.000120;-1.000000;-0.000100;,
  0.000300;-1.000000;-0.000060;,
  -0.000000;-1.000000;-0.000104;,
  -0.000120;-1.000000;-0.000100;,
  -0.000301;-1.000000;-0.000060;,
  -0.139411;0.990094;0.016695;,
  -0.619823;0.784742;0.000000;,
  -0.139411;-0.990094;0.016695;,
  -0.619823;-0.784742;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.273043;-0.961433;0.033074;,
  0.000009;-0.000066;1.000000;,
  0.000056;-0.000007;1.000000;,
  -0.456083;0.889937;0.000000;,
  0.000009;0.000066;1.000000;,
  -0.456083;-0.889937;0.000000;,
  0.139411;0.990094;0.016695;,
  0.619823;0.784742;0.000000;,
  0.139411;-0.990094;0.016695;,
  0.619823;-0.784742;-0.000000;,
  0.273043;-0.961433;0.033074;,
  -0.000009;-0.000066;1.000000;,
  -0.000073;0.000000;1.000000;,
  0.456083;0.889937;0.000000;,
  -0.000009;0.000066;1.000000;,
  0.456083;-0.889937;-0.000000;,
  0.705566;0.705566;-0.065987;,
  0.705566;-0.705566;-0.065987;,
  -0.014523;0.000000;-0.999894;,
  -0.018577;0.023158;-0.999559;,
  0.018583;0.023162;-0.999559;,
  0.014528;0.000000;-0.999894;,
  -0.018576;-0.023157;-0.999559;,
  0.018582;-0.023160;-0.999559;,
  -0.000300;1.000000;0.000060;,
  -0.000372;1.000000;-0.000000;,
  0.000300;1.000000;0.000060;,
  0.000371;1.000000;-0.000000;,
  0.000300;1.000000;-0.000060;,
  0.000137;1.000000;-0.000097;,
  -0.000137;1.000000;-0.000097;,
  -0.000300;1.000000;-0.000060;,
  -0.000300;-1.000000;0.000060;,
  -0.000372;-1.000000;-0.000000;,
  0.000300;-1.000000;0.000060;,
  0.000371;-1.000000;-0.000000;,
  0.000137;-1.000000;-0.000097;,
  0.000300;-1.000000;-0.000060;,
  -0.000137;-1.000000;-0.000097;,
  -0.000300;-1.000000;-0.000060;,
  -0.273043;0.961433;0.033074;,
  0.000010;-0.000129;1.000000;,
  0.000010;0.000000;1.000000;,
  0.000010;0.000129;1.000000;,
  -0.000013;0.000018;-1.000000;,
  -0.000015;0.000000;-1.000000;,
  -0.000015;0.000032;-1.000000;,
  -0.000013;-0.000018;-1.000000;,
  -0.000015;-0.000032;-1.000000;,
  0.000069;-0.000056;1.000000;,
  0.000100;-0.000083;1.000000;,
  0.000009;-0.000121;1.000000;,
  0.000151;-0.000021;1.000000;,
  0.000142;0.000142;1.000000;,
  0.000097;0.000096;1.000000;,
  0.000000;0.000201;1.000000;,
  0.273043;0.961433;0.033074;,
  -0.000010;0.000000;1.000000;,
  -0.000010;-0.000129;1.000000;,
  -0.000010;0.000129;1.000000;,
  0.000015;0.000032;-1.000000;,
  0.000015;0.000000;-1.000000;,
  0.000013;0.000018;-1.000000;,
  0.000015;-0.000032;-1.000000;,
  0.000013;-0.000018;-1.000000;,
  -0.000142;-0.000142;1.000000;,
  -0.000097;-0.000096;1.000000;,
  -0.000201;0.000000;1.000000;,
  -0.000097;0.000096;1.000000;,
  -0.000142;0.000142;1.000000;,
  -0.018581;0.015821;-0.999702;,
  -0.018580;-0.015820;-0.999702;,
  0.018587;0.015824;-0.999702;,
  0.018586;-0.015823;-0.999702;,
  0.000000;0.026941;-0.999637;,
  0.000000;-0.026939;-0.999637;,
  -0.156946;0.987269;-0.025829;,
  -0.310053;0.950124;-0.033634;,
  0.000000;0.998298;-0.058323;,
  -0.156946;-0.987269;-0.025824;,
  0.156958;0.987267;-0.025830;,
  0.310076;0.950117;-0.033635;,
  0.156958;-0.987268;-0.025825;,
  0.157091;0.987584;-0.000005;,
  0.310282;0.950645;-0.000004;,
  0.157074;-0.987587;0.000003;,
  -0.157068;0.987588;-0.000003;,
  -0.310236;0.950660;-0.000001;,
  -0.157050;-0.987591;0.000005;,
  0.310076;-0.950117;-0.033633;,
  0.310249;-0.950655;0.000001;,
  -0.310203;-0.950670;0.000004;,
  -0.310053;-0.950124;-0.033632;,
  0.000000;-0.998298;-0.058323;;
  214;
  4;19,4,0,20;,
  4;19,20,12,11;,
  4;19,11,13,21;,
  4;19,21,3,4;,
  4;20,0,35,22;,
  4;20,22,14,12;,
  4;21,13,15,23;,
  4;21,23,92,3;,
  4;22,35,1,24;,
  4;22,24,16,14;,
  4;23,15,17,25;,
  4;23,25,91,92;,
  4;24,1,2,26;,
  4;24,26,18,16;,
  4;25,17,18,26;,
  4;25,26,2,91;,
  4;27,11,12,28;,
  4;27,28,5,10;,
  4;27,10,9,29;,
  4;27,29,13,11;,
  4;28,12,14,30;,
  4;28,30,157,5;,
  4;29,9,8,31;,
  4;29,31,15,13;,
  4;30,14,16,32;,
  4;30,32,6,157;,
  4;31,8,7,33;,
  4;31,33,17,15;,
  4;32,16,18,34;,
  4;32,34,158,6;,
  4;33,7,158,34;,
  4;33,34,18,17;,
  3;36,38,37;,
  3;36,39,38;,
  3;36,41,40;,
  3;36,37,41;,
  4;37,38,43,42;,
  4;38,39,44,43;,
  4;40,41,46,45;,
  4;41,37,42,46;,
  4;43,44,48,47;,
  4;45,46,50,49;,
  4;47,48,52,51;,
  4;49,50,54,53;,
  4;51,52,56,55;,
  4;53,54,58,57;,
  4;55,56,61,60;,
  4;57,58,63,62;,
  4;59,60,35,0;,
  4;60,61,1,35;,
  4;62,63,4,3;,
  4;63,59,0,4;,
  3;74,72,73;,
  3;75,72,74;,
  3;76,72,75;,
  3;77,72,76;,
  3;78,72,77;,
  3;79,72,78;,
  3;80,73,72;,
  3;80,72,79;,
  4;74,73,64,71;,
  4;75,74,71,67;,
  4;67,68,76,75;,
  4;77,76,68,69;,
  4;69,70,78,77;,
  4;70,66,79,78;,
  4;80,79,66,65;,
  4;80,65,64,73;,
  4;81,60,59,82;,
  4;81,82,68,67;,
  4;81,67,71,83;,
  4;81,83,165,60;,
  4;82,59,63,84;,
  4;82,84,69,68;,
  3;83,166,165;,
  3;83,71,166;,
  4;84,63,167,85;,
  4;84,85,70,69;,
  3;85,168,70;,
  3;85,167,168;,
  4;86,66,70,87;,
  4;86,87,169,170;,
  4;86,170,42,88;,
  4;86,88,65,66;,
  3;87,168,169;,
  3;87,70,168;,
  4;88,42,171,89;,
  4;88,89,64,65;,
  4;89,171,172,90;,
  4;89,90,71,64;,
  3;90,166,71;,
  3;90,172,166;,
  3;93,94,36;,
  3;94,39,36;,
  3;40,95,36;,
  3;95,93,36;,
  4;96,97,94,93;,
  4;97,44,39,94;,
  4;45,98,95,40;,
  4;98,96,93,95;,
  4;99,48,44,97;,
  4;49,100,98,45;,
  4;101,52,48,99;,
  4;53,102,100,49;,
  4;103,56,52,101;,
  4;57,104,102,53;,
  4;106,61,56,103;,
  4;62,107,104,57;,
  4;91,2,106,105;,
  4;2,1,61,106;,
  4;3,92,107,62;,
  4;92,91,105,107;,
  3;117,116,118;,
  3;118,116,119;,
  3;119,116,120;,
  3;120,116,121;,
  3;121,116,122;,
  3;122,116,123;,
  3;116,117,124;,
  3;123,116,124;,
  4;115,108,117,118;,
  4;111,115,118,119;,
  4;119,120,112,111;,
  4;113,112,120,121;,
  4;121,122,114,113;,
  4;122,123,110,114;,
  4;109,110,123,124;,
  4;117,108,109,124;,
  4;126,105,106,125;,
  4;111,112,126,125;,
  4;127,115,111,125;,
  4;106,173,127,125;,
  4;128,107,105,126;,
  4;112,113,128,126;,
  3;173,174,127;,
  3;174,115,127;,
  4;129,175,107,128;,
  4;113,114,129,128;,
  3;114,176,129;,
  3;176,175,129;,
  4;131,114,110,130;,
  4;177,178,131,130;,
  4;132,96,177,130;,
  4;110,109,132,130;,
  3;178,176,131;,
  3;176,114,131;,
  4;133,179,96,132;,
  4;109,108,133,132;,
  4;134,180,179,133;,
  4;108,115,134,133;,
  3;115,174,134;,
  3;174,180,134;,
  4;139,136,136,139;,
  4;140,140,138,138;,
  4;141,137,137,141;,
  4;181,181,135,135;,
  4;142,182,183,143;,
  4;144,136,136,144;,
  4;144,144,135,135;,
  4;143,183,184,145;,
  4;185,159,186,187;,
  4;146,137,137,146;,
  4;146,146,138,138;,
  4;159,188,189,186;,
  4;190,191,192,192;,
  4;191,190,143,193;,
  4;194,193,143,195;,
  4;194,195,196,196;,
  4;139,148,148,139;,
  4;150,150,140,140;,
  4;151,149,149,151;,
  4;147,147,197,197;,
  4;153,198,199,152;,
  4;154,148,148,154;,
  4;147,147,154,154;,
  4;155,200,198,153;,
  4;201,202,162,203;,
  4;156,149,149,156;,
  4;150,150,156,156;,
  4;202,204,205,162;,
  4;192,192,206,207;,
  4;208,153,207,206;,
  4;209,153,208,210;,
  4;196,196,209,210;,
  3;143,190,142;,
  3;143,145,195;,
  3;159,185,211;,
  3;159,212,188;,
  3;162,213,203;,
  3;162,205,214;,
  3;153,152,207;,
  3;153,209,155;,
  3;159,211,160;,
  3;162,161,213;,
  3;215,160,211;,
  3;215,213,161;,
  3;161,160,215;,
  3;159,163,212;,
  3;163,216,212;,
  3;164,162,214;,
  3;214,216,164;,
  3;163,164,216;,
  4;221,224,225,222;,
  4;230,231,226,223;,
  4;227,217,218,228;,
  4;232,233,220,229;,
  4;227,224,221,217;,
  4;220,223,226,229;,
  3;217,219,218;,
  3;222,219,221;,
  3;233,234,220;,
  3;223,234,230;,
  3;219,217,221;,
  3;234,223,220;;
 }
 MeshTextureCoords {
  276;
  0.875000;0.250000;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  1.000000;0.500000;,
  0.875000;0.500000;,
  0.750000;0.500000;,
  0.750000;0.250000;,
  0.750000;0.000000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.250000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.625000;0.500000;,
  0.625000;0.250000;,
  0.625000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.250000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.375000;0.000000;,
  0.375000;0.250000;,
  0.375000;0.500000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.875000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.750000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.250000;0.750000;,
  0.250000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.750000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.752778;0.500000;,
  0.707758;0.413361;,
  0.752778;0.409966;,
  0.690706;0.500000;,
  0.797798;0.413361;,
  0.814849;0.500000;,
  0.655568;0.334251;,
  0.752778;0.305585;,
  0.628571;0.500000;,
  0.849988;0.334251;,
  0.876984;0.500000;,
  0.566317;0.500000;,
  0.588394;0.314911;,
  0.917162;0.314911;,
  0.939239;0.500000;,
  0.503905;0.500000;,
  0.504372;0.304090;,
  1.001183;0.304090;,
  1.001650;0.500000;,
  0.441322;0.500000;,
  0.419734;0.313526;,
  0.001650;0.500000;,
  0.001183;0.304090;,
  0.085821;0.313526;,
  0.064233;0.500000;,
  0.378579;0.500000;,
  0.351491;0.332410;,
  0.154064;0.332410;,
  0.126976;0.500000;,
  0.252778;0.302590;,
  0.299340;0.410623;,
  0.252778;0.371820;,
  0.316868;0.500000;,
  0.206215;0.410623;,
  0.188688;0.500000;,
  0.508719;0.096128;,
  0.752778;0.003701;,
  0.631872;0.098476;,
  0.382094;0.093707;,
  0.252778;0.093035;,
  0.123462;0.093707;,
  -0.247222;0.003701;,
  0.996836;0.096128;,
  1.123462;0.093707;,
  0.873684;0.098476;,
  0.752778;0.099773;,
  0.629510;0.237626;,
  0.505255;0.235899;,
  0.379549;0.234136;,
  0.252778;0.234012;,
  0.126007;0.234136;,
  0.000301;0.235899;,
  -0.003164;0.096128;,
  -0.123955;0.237626;,
  -0.126316;0.098476;,
  0.876045;0.237626;,
  0.752778;0.238922;,
  0.361151;0.292248;,
  0.252778;0.272478;,
  0.451798;0.274929;,
  0.144404;0.292248;,
  0.053758;0.274929;,
  0.859011;0.294827;,
  1.000301;0.235899;,
  0.948157;0.276167;,
  0.752778;0.276299;,
  0.646544;0.294827;,
  0.557398;0.276167;,
  0.752778;0.590034;,
  0.707758;0.586639;,
  0.797798;0.586639;,
  0.752778;0.694415;,
  0.655568;0.665749;,
  0.849988;0.665749;,
  0.588394;0.685089;,
  0.917162;0.685089;,
  0.504372;0.695910;,
  0.001183;0.695910;,
  -0.082838;0.685089;,
  -0.060761;0.500000;,
  0.419734;0.686474;,
  0.085821;0.686474;,
  0.351491;0.667590;,
  0.154064;0.667590;,
  0.252778;0.628180;,
  0.299340;0.589377;,
  0.252778;0.697410;,
  0.206215;0.589377;,
  0.631872;0.901524;,
  0.752778;0.996299;,
  0.508719;0.903872;,
  0.382094;0.906293;,
  0.252778;0.906965;,
  0.123462;0.906293;,
  -0.247222;0.996299;,
  -0.003164;0.903872;,
  0.996836;0.903872;,
  0.873684;0.901524;,
  0.752778;0.900227;,
  0.505255;0.764101;,
  0.629510;0.762374;,
  0.379549;0.765864;,
  0.252778;0.765988;,
  0.126007;0.765864;,
  0.000301;0.764101;,
  0.876045;0.762374;,
  1.000301;0.764101;,
  0.752778;0.761078;,
  0.252778;0.727522;,
  0.361151;0.707752;,
  0.451798;0.725071;,
  0.144404;0.707752;,
  0.053758;0.725071;,
  0.948157;0.723833;,
  0.859011;0.705173;,
  0.752778;0.723701;,
  1.001183;0.695910;,
  -0.051843;0.723833;,
  0.646544;0.705173;,
  0.557398;0.723833;,
  0.488945;1.000000;,
  0.411315;1.000000;,
  0.411315;0.000000;,
  0.488945;0.000000;,
  0.488945;0.000000;,
  0.488945;1.000000;,
  0.411315;1.000000;,
  0.411315;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.681946;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.681946;0.500000;,
  0.318054;0.000000;,
  0.333333;0.000000;,
  0.333333;1.000000;,
  0.318054;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.681946;1.000000;,
  0.318054;0.000000;,
  0.318054;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.318054;1.000000;,
  0.000000;0.000000;,
  0.318054;0.000000;,
  0.333333;0.000000;,
  0.333333;1.000000;,
  0.318054;1.000000;,
  0.000000;1.000000;,
  0.666667;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.666667;0.500000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.666667;1.000000;,
  0.000000;0.000000;,
  0.411315;0.000000;,
  0.411315;1.000000;,
  0.411315;0.000000;,
  0.411315;1.000000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.681946;0.500000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.681946;0.000000;,
  0.318054;1.000000;,
  0.333333;1.000000;,
  0.333333;0.000000;,
  0.318054;0.000000;,
  0.000000;1.000000;,
  0.681946;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.318054;0.500000;,
  0.318054;0.000000;,
  0.318054;0.000000;,
  0.000000;0.000000;,
  0.318054;1.000000;,
  0.333333;1.000000;,
  0.333333;0.000000;,
  0.000000;1.000000;,
  0.318054;1.000000;,
  0.000000;0.000000;,
  0.666667;0.000000;,
  0.000000;0.000000;,
  0.666667;0.500000;,
  0.666667;1.000000;,
  0.000000;0.000000;,
  0.674306;0.500000;,
  0.325694;0.500000;,
  0.333333;0.000000;,
  0.333333;1.000000;,
  0.325694;0.500000;,
  0.333333;0.000000;,
  0.333333;1.000000;,
  0.674306;0.500000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.333333;0.500000;,
  0.666667;0.500000;,
  0.500000;0.500000;;
 }
}
