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
 69;
 -147.37230;-50.54160;-221.02860;,
 -56.20230;-50.54160;-286.78711;,
 -56.20230;-129.66600;-286.78711;,
 -147.37230;-129.66600;-221.02860;,
 56.21160;-50.54160;-286.78711;,
 56.21160;-129.66600;-286.78711;,
 147.38100;-50.54160;-221.02860;,
 147.38100;-129.66600;-221.02860;,
 130.10460;-0.01320;68.80260;,
 56.21160;-0.01320;163.43430;,
 56.21160;-129.66600;163.43430;,
 162.03030;-129.66600;68.80260;,
 -56.20230;-0.01320;163.43430;,
 -56.20230;-129.66600;163.43430;,
 -130.09501;-0.01320;68.80260;,
 -162.02100;-129.66600;68.80260;,
 -130.09501;-0.01320;-143.49810;,
 -130.09501;-0.01320;68.80260;,
 -56.20230;-0.01320;163.43430;,
 -56.20230;-0.01320;-143.49810;,
 -56.20230;-50.54160;-286.78711;,
 -147.37230;-50.54160;-221.02860;,
 -130.09501;-0.01320;-143.49810;,
 -147.37230;-50.54160;-221.02860;,
 -147.37230;-129.66600;-221.02860;,
 -162.02100;-129.66600;-143.49810;,
 -162.02100;-129.66600;68.80260;,
 56.21160;-0.01320;163.43430;,
 56.21160;-0.01320;-143.49810;,
 56.21160;-50.54160;-286.78711;,
 -162.02100;-129.66600;-143.49810;,
 -147.37230;-129.66600;-221.02860;,
 -56.20230;-129.66600;-286.78711;,
 -56.20230;-129.66600;-143.49810;,
 -56.20230;-129.66600;163.43430;,
 130.10460;-0.01320;68.80260;,
 130.10460;-0.01320;-143.49810;,
 147.38100;-50.54160;-221.02860;,
 56.21160;-129.66600;-286.78711;,
 56.21160;-129.66600;-143.49810;,
 56.21160;-129.66600;163.43430;,
 130.10460;-0.01320;-143.49810;,
 162.03030;-129.66600;68.80260;,
 162.03030;-129.66600;-143.49810;,
 147.38100;-129.66600;-221.02860;,
 147.38100;-50.54160;-221.02860;,
 147.38100;-129.66600;-221.02860;,
 162.03030;-129.66600;-143.49810;,
 0.00450;-78.56850;-270.49919;,
 0.00450;-129.65730;-455.45100;,
 -40.22490;-128.04630;-270.49560;,
 0.00450;-129.65730;-455.45100;,
 40.23420;-128.04630;-270.49560;,
 0.00450;-129.65730;-455.45100;,
 0.00450;-78.56850;-270.49919;,
 -100.80390;-78.56850;-214.51980;,
 -100.80390;-129.65730;-399.47131;,
 -141.03300;-128.04630;-214.51620;,
 -100.80390;-129.65730;-399.47131;,
 -60.57420;-128.04630;-214.51620;,
 -100.80390;-129.65730;-399.47131;,
 -100.80390;-78.56850;-214.51980;,
 141.04259;-128.04630;-214.51620;,
 100.81320;-129.65730;-399.47131;,
 100.81320;-78.56850;-214.51980;,
 60.58350;-128.04630;-214.51620;,
 100.81320;-129.65730;-399.47131;,
 100.81320;-78.56850;-214.51980;,
 100.81320;-129.65730;-399.47131;;
 
 34;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;12,14,15,13;,
 4;16,17,18,19;,
 4;16,19,20,21;,
 4;22,23,24,25;,
 4;22,25,26,17;,
 4;19,18,27,28;,
 4;19,28,29,20;,
 4;30,31,32,33;,
 4;30,33,34,26;,
 4;28,27,35,36;,
 4;28,36,37,29;,
 4;33,32,38,39;,
 4;33,39,40,34;,
 4;41,35,42,43;,
 4;41,43,44,45;,
 4;39,38,46,47;,
 4;39,47,42,40;,
 3;48,49,50;,
 3;50,51,52;,
 3;52,53,54;,
 3;50,52,48;,
 3;55,56,57;,
 3;57,58,59;,
 3;59,60,61;,
 3;57,59,55;,
 3;62,63,64;,
 3;65,66,62;,
 3;67,68,65;,
 3;64,65,62;;
 
 MeshMaterialList {
  3;
  34;
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
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Rock003.png";
   }
  }
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  32;
  -0.872361;0.067065;-0.484239;,
  -0.307373;0.000000;-0.951589;,
  0.307374;0.000000;-0.951589;,
  0.872360;0.067064;-0.484242;,
  0.914413;0.172565;0.366157;,
  0.397247;0.044499;0.916632;,
  -0.397248;0.044500;0.916632;,
  -0.914413;0.172566;0.366156;,
  -0.060834;0.972053;-0.226743;,
  -0.030233;0.979797;-0.197698;,
  -0.983054;0.180528;-0.031847;,
  0.030233;0.979797;-0.197698;,
  0.000000;-1.000000;-0.000000;,
  0.060834;0.972053;-0.226743;,
  0.983054;0.180526;-0.031852;,
  -0.764381;0.621490;-0.171673;,
  0.000000;-0.999962;0.008710;,
  -0.764383;0.621488;-0.171672;,
  0.000000;-0.999962;0.008710;,
  0.764381;0.621490;-0.171673;,
  0.000000;-0.999962;0.008710;,
  0.000000;1.000000;0.000000;,
  -0.059346;0.919715;-0.388074;,
  -0.118267;0.889774;-0.440813;,
  0.059346;0.919715;-0.388074;,
  0.118268;0.889774;-0.440813;,
  0.764379;0.621493;-0.171674;,
  0.000000;0.000073;1.000000;,
  0.764379;0.621493;-0.171674;,
  0.000000;0.000073;1.000000;,
  -0.764378;0.621493;-0.171674;,
  0.000000;0.000073;1.000000;;
  34;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;8,21,21,9;,
  4;8,9,22,23;,
  4;10,0,0,10;,
  4;10,10,7,7;,
  4;9,21,21,11;,
  4;9,11,24,22;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;11,21,21,13;,
  4;11,13,25,24;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  4;14,4,4,14;,
  4;14,14,3,3;,
  4;12,12,12,12;,
  4;12,12,12,12;,
  3;15,15,15;,
  3;16,16,16;,
  3;26,26,26;,
  3;27,27,27;,
  3;17,17,17;,
  3;18,18,18;,
  3;28,28,28;,
  3;29,29,29;,
  3;19,19,19;,
  3;20,20,20;,
  3;30,30,30;,
  3;31,31,31;;
 }
 MeshTextureCoords {
  69;
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.500000;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.666670;0.000000;,
  0.666670;0.500000;,
  0.666670;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.500000;,
  0.333330;1.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.666670;0.000000;,
  0.666670;0.500000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.333330;1.000000;,
  0.166670;0.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  0.333330;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;;
 }
}
