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
 92;
 -56.82630;741.11595;-331.48501;,
 -39.58440;748.12408;-340.28684;,
 -43.20855;491.97990;-354.70816;,
 -80.25450;491.97990;-337.92991;,
 42.32940;748.12408;-340.28684;,
 45.95355;491.97990;-354.70816;,
 57.39720;743.34389;-331.48501;,
 80.25540;491.97990;-337.92991;,
 -48.62745;192.10046;-359.99236;,
 -96.51134;192.10046;-343.21410;,
 51.37245;192.10046;-359.99236;,
 96.51225;192.10046;-343.21410;,
 -48.62745;0.00000;-359.99236;,
 -82.27425;0.00000;-343.21410;,
 51.37245;0.00000;-359.99236;,
 82.27515;0.00000;-343.21410;,
 57.39720;743.34389;-331.48501;,
 61.29900;753.05846;-115.49475;,
 80.25540;491.97990;-115.49475;,
 80.25540;491.97990;-337.92991;,
 61.29900;753.05846;112.22475;,
 80.25540;491.97990;112.22475;,
 59.77980;743.35066;291.34321;,
 80.25540;491.97990;324.93480;,
 96.51225;192.10046;-115.49475;,
 96.51225;192.10046;-343.21410;,
 96.51225;192.10046;112.22475;,
 96.51225;192.10046;339.94409;,
 96.51225;0.00000;-115.49475;,
 82.27515;0.00000;-343.21410;,
 96.51225;0.00000;112.22475;,
 82.27515;0.00000;339.94409;,
 59.77980;743.35066;291.34321;,
 42.32940;748.13095;310.36139;,
 45.95355;491.97990;344.97029;,
 80.25540;491.97990;324.93480;,
 -39.58440;748.13095;310.36139;,
 -43.20855;491.97990;344.97029;,
 -59.20890;741.12282;291.34321;,
 -80.25450;491.97990;324.93480;,
 51.37245;192.10046;359.97961;,
 96.51225;192.10046;339.94409;,
 -48.62745;192.10046;359.97961;,
 -96.51134;192.10046;339.94409;,
 51.37245;0.00000;359.97961;,
 82.27515;0.00000;339.94409;,
 -48.62745;0.00000;359.97961;,
 -82.27425;0.00000;339.94409;,
 -59.20890;741.12282;291.34321;,
 -63.46335;751.81645;112.22475;,
 -80.25450;491.97990;112.22475;,
 -80.25450;491.97990;324.93480;,
 -63.46335;751.81645;-115.49475;,
 -80.25450;491.97990;-115.49475;,
 -56.82630;741.11595;-331.48501;,
 -80.25450;491.97990;-337.92991;,
 -96.51134;192.10046;112.22475;,
 -96.51134;192.10046;339.94409;,
 -96.51134;192.10046;-115.49475;,
 -96.51134;192.10046;-343.21410;,
 -96.51134;0.00000;112.22475;,
 -82.27425;0.00000;339.94409;,
 -96.51134;0.00000;-115.49475;,
 -82.27425;0.00000;-343.21410;,
 -39.58440;748.13095;310.36139;,
 -39.58440;765.51416;112.22475;,
 -63.46335;751.81645;112.22475;,
 42.32940;748.13095;310.36139;,
 42.32940;765.51416;112.22475;,
 61.29900;753.05846;112.22475;,
 -39.58440;765.51416;-115.49475;,
 -63.46335;751.81645;-115.49475;,
 42.32940;765.51416;-115.49475;,
 61.29900;753.05846;-115.49475;,
 -39.58440;748.12408;-340.28684;,
 -56.82630;741.11595;-331.48501;,
 42.32940;748.12408;-340.28684;,
 57.39720;743.34389;-331.48501;,
 -96.51134;0.00000;112.22475;,
 -48.62745;0.00000;112.22475;,
 -48.62745;0.00000;359.97961;,
 51.37245;0.00000;112.22475;,
 51.37245;0.00000;359.97961;,
 96.51225;0.00000;112.22475;,
 -96.51134;0.00000;-115.49475;,
 -48.62745;0.00000;-115.49475;,
 51.37245;0.00000;-115.49475;,
 96.51225;0.00000;-115.49475;,
 -82.27425;0.00000;-343.21410;,
 -48.62745;0.00000;-359.99236;,
 51.37245;0.00000;-359.99236;,
 82.27515;0.00000;-343.21410;;
 
 54;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;3,2,8,9;,
 4;2,5,10,8;,
 4;5,7,11,10;,
 4;9,8,12,13;,
 4;8,10,14,12;,
 4;10,11,15,14;,
 4;16,17,18,19;,
 4;17,20,21,18;,
 4;20,22,23,21;,
 4;19,18,24,25;,
 4;18,21,26,24;,
 4;21,23,27,26;,
 4;25,24,28,29;,
 4;24,26,30,28;,
 4;26,27,31,30;,
 4;32,33,34,35;,
 4;33,36,37,34;,
 4;36,38,39,37;,
 4;35,34,40,41;,
 4;34,37,42,40;,
 4;37,39,43,42;,
 4;41,40,44,45;,
 4;40,42,46,44;,
 4;42,43,47,46;,
 4;48,49,50,51;,
 4;49,52,53,50;,
 4;52,54,55,53;,
 4;51,50,56,57;,
 4;50,53,58,56;,
 4;53,55,59,58;,
 4;57,56,60,61;,
 4;56,58,62,60;,
 4;58,59,63,62;,
 4;48,64,65,66;,
 4;64,67,68,65;,
 4;67,22,69,68;,
 4;66,65,70,71;,
 4;65,68,72,70;,
 4;68,69,73,72;,
 4;71,70,74,75;,
 4;70,72,76,74;,
 4;72,73,77,76;,
 4;78,79,80,61;,
 4;79,81,82,80;,
 4;81,83,31,82;,
 4;84,85,79,78;,
 4;85,86,81,79;,
 4;86,87,83,81;,
 4;88,89,85,84;,
 4;89,90,86,85;,
 4;90,91,87,86;;
 
 MeshMaterialList {
  3;
  54;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
    "data\\TEXTURE\\Wall.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\WallGrass.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\WallGround.png";
   }
  }
 }
 MeshNormals {
  69;
  -0.442124;0.059260;-0.894994;,
  -0.226990;0.059284;-0.972091;,
  0.246659;0.059546;-0.967271;,
  -0.407332;0.044192;-0.912210;,
  -0.208163;0.041465;-0.977215;,
  0.223920;0.041750;-0.973713;,
  -0.380622;0.008388;-0.924693;,
  -0.193975;0.008765;-0.980967;,
  0.206484;0.008762;-0.978411;,
  -0.389213;-0.012275;-0.921066;,
  -0.198565;-0.006262;-0.980068;,
  0.211697;-0.006620;-0.977313;,
  0.478070;0.059198;-0.876325;,
  0.846103;0.532640;-0.020105;,
  0.847163;0.530935;0.020544;,
  0.436672;0.044457;-0.898522;,
  0.997846;0.065548;-0.002659;,
  0.997925;0.064374;0.001548;,
  0.404116;0.008337;-0.914670;,
  0.999810;0.017849;-0.007794;,
  0.999810;0.017849;0.007794;,
  0.413788;-0.012940;-0.910281;,
  0.999708;-0.018469;-0.015580;,
  0.999708;-0.018469;0.015580;,
  0.832864;0.439625;0.336254;,
  0.334239;0.139344;0.932131;,
  -0.314648;0.140035;0.938822;,
  0.876599;0.090974;0.472545;,
  0.283974;0.097539;0.953858;,
  -0.267641;0.097805;0.958542;,
  0.999841;0.008613;0.015593;,
  0.239942;0.024460;0.970479;,
  -0.226105;0.024517;0.973794;,
  0.998832;-0.036927;0.031151;,
  0.245681;-0.007776;0.969320;,
  -0.231218;-0.007367;0.972874;,
  -0.824458;0.441954;0.353477;,
  -0.840387;0.541435;0.024444;,
  -0.839934;0.542192;-0.023201;,
  -0.867319;0.091852;0.489204;,
  -0.998084;0.061773;0.003443;,
  -0.998007;0.062957;-0.004231;,
  -0.440589;0.022761;0.897421;,
  -0.999810;0.017849;0.007794;,
  -0.999810;0.017849;-0.007794;,
  -0.449891;-0.014335;0.892968;,
  -0.999708;-0.018469;0.015580;,
  -0.999708;-0.018469;-0.015580;,
  -0.244676;0.968734;0.041095;,
  0.258886;0.965085;0.039868;,
  -0.245081;0.968814;-0.036526;,
  0.258116;0.965456;-0.035645;,
  0.000000;-1.000000;-0.000000;,
  0.828317;0.558774;-0.040763;,
  0.997684;0.067818;-0.005318;,
  0.999841;0.008613;-0.015593;,
  0.998832;-0.036927;-0.031151;,
  0.465967;0.022488;0.884516;,
  0.476286;-0.015075;0.879161;,
  -0.830399;0.555216;-0.046614;,
  -0.997744;0.066602;-0.008463;,
  -0.999841;0.008613;0.015593;,
  -0.999841;0.008613;-0.015593;,
  -0.998832;-0.036927;0.031151;,
  -0.998832;-0.036927;-0.031151;,
  -0.231518;0.969382;0.081838;,
  0.231148;0.969703;0.079036;,
  -0.232410;0.969892;-0.072760;,
  0.229700;0.970693;-0.070662;;
  54;
  4;0,1,4,3;,
  4;1,2,5,4;,
  4;2,12,15,5;,
  4;3,4,7,6;,
  4;4,5,8,7;,
  4;5,15,18,8;,
  4;6,7,10,9;,
  4;7,8,11,10;,
  4;8,18,21,11;,
  4;53,13,16,54;,
  4;13,14,17,16;,
  4;14,24,27,17;,
  4;54,16,19,55;,
  4;16,17,20,19;,
  4;17,27,30,20;,
  4;55,19,22,56;,
  4;19,20,23,22;,
  4;20,30,33,23;,
  4;24,25,28,27;,
  4;25,26,29,28;,
  4;26,36,39,29;,
  4;27,28,31,57;,
  4;28,29,32,31;,
  4;29,39,42,32;,
  4;57,31,34,58;,
  4;31,32,35,34;,
  4;32,42,45,35;,
  4;36,37,40,39;,
  4;37,38,41,40;,
  4;38,59,60,41;,
  4;39,40,43,61;,
  4;40,41,44,43;,
  4;41,60,62,44;,
  4;61,43,46,63;,
  4;43,44,47,46;,
  4;44,62,64,47;,
  4;36,65,48,37;,
  4;65,66,49,48;,
  4;66,24,14,49;,
  4;37,48,50,38;,
  4;48,49,51,50;,
  4;49,14,13,51;,
  4;38,50,67,59;,
  4;50,51,68,67;,
  4;51,13,53,68;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  4;52,52,52,52;;
 }
 MeshTextureCoords {
  92;
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.333330;0.000000;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  1.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.000000;0.666670;,
  0.333330;0.666670;,
  0.333330;1.000000;,
  0.666670;0.666670;,
  0.666670;1.000000;,
  1.000000;0.666670;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.666670;0.333330;,
  1.000000;0.333330;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.666670;0.000000;,
  1.000000;0.000000;;
 }
}
