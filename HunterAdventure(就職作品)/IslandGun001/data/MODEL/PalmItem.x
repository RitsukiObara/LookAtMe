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
 219;
 -64.70579;19.12207;-19.13415;,
 -74.27288;28.68916;-13.52990;,
 -76.50723;42.67597;-25.00000;,
 -58.82955;24.99830;-35.35535;,
 -78.23568;32.65195;0.00000;,
 -83.82955;49.99831;0.00000;,
 -74.27288;28.68916;13.52990;,
 -76.50723;42.67597;25.00000;,
 -64.70579;19.12207;19.13415;,
 -58.82955;24.99830;35.35535;,
 -55.13873;9.55499;13.52990;,
 -41.15194;7.32063;25.00000;,
 -51.17595;5.59218;0.00000;,
 -33.82959;-0.00171;0.00000;,
 -55.13873;9.55499;-13.52990;,
 -41.15194;7.32063;-25.00000;,
 -64.70579;19.12207;-19.13415;,
 -58.82955;24.99830;-35.35535;,
 -71.90597;58.11582;-32.66405;,
 -48.80905;35.01884;-46.19395;,
 -81.47305;67.68291;0.00000;,
 -71.90597;58.11582;32.66405;,
 -48.80905;35.01884;46.19395;,
 -25.71207;11.92183;32.66405;,
 -16.14504;2.35479;0.00000;,
 -25.71207;11.92183;-32.66405;,
 -48.80905;35.01884;-46.19395;,
 -63.29245;70.53541;-35.35535;,
 -38.29251;45.53540;-50.00000;,
 -73.64779;80.89074;0.00000;,
 -63.29245;70.53541;35.35535;,
 -38.29251;45.53540;50.00000;,
 -13.29253;20.53539;35.35535;,
 -2.93721;10.18006;0.00000;,
 -13.29253;20.53539;-35.35535;,
 -38.29251;45.53540;-50.00000;,
 -65.31596;14.58442;2.77540;,
 -64.51861;13.76701;-0.00280;,
 -67.28043;16.54891;3.92620;,
 -69.24796;18.49635;2.76760;,
 -70.05939;19.30779;-0.00280;,
 -69.24796;18.49635;-2.77315;,
 -67.28899;16.53738;-3.92070;,
 -55.13873;9.55499;-13.52990;,
 -65.31596;14.58442;-2.78100;,
 -65.31596;14.58442;-2.78100;,
 -67.57873;16.24765;-0.00280;,
 -70.05939;19.30779;-0.00280;,
 -69.24796;18.49635;-2.77315;,
 -67.28899;16.53738;-3.92070;,
 -67.57873;16.24765;-0.00280;,
 -60.80935;23.01853;-16.31154;,
 -55.79995;28.02791;-30.13985;,
 -70.86985;43.09783;-21.31207;,
 -68.96511;31.17430;-11.53401;,
 -77.11199;49.33998;0.00000;,
 -72.34333;34.55253;0.00000;,
 -70.86985;43.09783;21.31207;,
 -68.96511;31.17430;11.53401;,
 -55.79995;28.02791;30.13985;,
 -60.80935;23.01853;16.31154;,
 -40.73008;12.95801;21.31207;,
 -52.65360;14.86277;11.53401;,
 -34.48789;6.71582;0.00000;,
 -49.27538;11.48454;0.00000;,
 -40.73008;12.95801;-21.31207;,
 -52.65360;14.86277;-11.53401;,
 -55.79995;28.02791;-30.13985;,
 -60.80935;23.01853;-16.31154;,
 -47.25764;36.57026;-39.37957;,
 -66.94738;56.26005;-27.84556;,
 -75.10314;64.41582;0.00000;,
 -66.94738;56.26005;27.84556;,
 -47.25764;36.57026;39.37957;,
 -27.56786;16.88045;27.84556;,
 -19.41213;8.72471;0.00000;,
 -27.56786;16.88045;-27.84556;,
 -47.25764;36.57026;-39.37957;,
 -38.29246;45.53545;-42.62415;,
 -59.60448;66.84752;-30.13985;,
 -68.43221;75.67528;0.00000;,
 -59.60448;66.84752;30.13985;,
 -38.29246;45.53545;42.62415;,
 -16.98040;24.22335;30.13985;,
 -8.15267;15.39562;0.00000;,
 -16.98040;24.22335;-30.13985;,
 -38.29246;45.53545;-42.62415;,
 -60.64979;18.45343;-0.00239;,
 -61.32950;19.15026;2.36598;,
 -63.00418;20.82496;3.34702;,
 -64.68147;22.48512;2.35933;,
 -65.37319;23.17685;-0.00239;,
 -64.68147;22.48512;-2.36406;,
 -63.01147;20.81512;-3.34233;,
 -61.32950;19.15026;-2.37076;,
 -52.65360;14.86277;-11.53401;,
 -61.32950;19.15026;-2.37076;,
 -63.25847;20.56812;-0.00239;,
 -64.68147;22.48512;-2.36406;,
 -65.37319;23.17685;-0.00239;,
 -63.01147;20.81512;-3.34233;,
 -63.25847;20.56812;-0.00239;,
 -72.02242;79.26543;0.00000;,
 -73.64779;80.89074;0.00000;,
 -63.29245;70.53541;35.35535;,
 -62.14314;69.38615;33.73003;,
 -62.14314;69.38615;-33.73003;,
 -38.29251;45.53540;50.00000;,
 -38.29248;45.53543;47.70144;,
 -38.29248;45.53543;-47.70144;,
 -13.29253;20.53539;35.35535;,
 -14.44179;21.68469;33.73003;,
 -14.44179;21.68469;-33.73003;,
 -14.44179;21.68469;33.73003;,
 -4.56252;11.80541;0.00000;,
 37.79905;54.82156;-50.00000;,
 62.79904;79.82156;-35.35535;,
 74.42592;64.38861;-32.66405;,
 51.32897;41.29164;-46.19395;,
 73.15437;90.17690;0.00000;,
 83.99300;73.95569;0.00000;,
 62.79904;79.82156;35.35535;,
 74.42592;64.38861;32.66405;,
 37.79905;54.82156;50.00000;,
 51.32897;41.29164;46.19395;,
 12.79904;29.82156;35.35535;,
 28.23196;18.19464;32.66405;,
 2.44371;19.46623;0.00000;,
 18.66491;8.62759;0.00000;,
 12.79904;29.82156;-35.35535;,
 28.23196;18.19464;-32.66405;,
 37.79905;54.82156;-50.00000;,
 51.32897;41.29164;-46.19395;,
 85.29858;42.67732;-25.00000;,
 67.62094;24.99965;-35.35535;,
 92.62093;49.99966;0.00000;,
 85.29858;42.67732;25.00000;,
 67.62094;24.99965;35.35535;,
 49.94328;7.32198;25.00000;,
 42.62095;-0.00035;-0.00000;,
 49.94328;7.32198;-25.00000;,
 67.62094;24.99965;-35.35535;,
 87.95763;23.79710;-13.52990;,
 78.39055;14.23002;-19.13415;,
 91.92044;27.75991;0.00000;,
 87.95763;23.79710;13.52990;,
 78.39055;14.23002;19.13415;,
 68.82347;4.66294;13.52990;,
 64.86067;0.70014;0.00000;,
 68.82347;4.66294;-13.52990;,
 78.39055;14.23002;-19.13415;,
 80.75618;11.86440;-4.46040;,
 81.08180;11.53877;0.00000;,
 78.52599;9.63422;-3.15395;,
 82.98636;14.09458;-3.15395;,
 83.91014;15.01838;0.00000;,
 82.98636;14.09458;3.15395;,
 81.08180;11.53877;0.00000;,
 80.75618;11.86440;4.46040;,
 78.52599;9.63422;3.15395;,
 77.60219;8.71042;0.00000;,
 78.52599;9.63422;-3.15395;,
 80.75618;11.86440;-4.46040;,
 37.79857;54.82204;-42.62500;,
 49.33282;43.28778;-39.38034;,
 69.02296;62.97795;-27.84610;,
 59.11107;76.13455;-30.14044;,
 77.17892;71.13389;0.00000;,
 67.93898;84.96246;0.00000;,
 69.02296;62.97795;27.84610;,
 59.11107;76.13455;30.14044;,
 49.33282;43.28778;39.38034;,
 37.79857;54.82204;42.62500;,
 29.64262;23.59759;27.84610;,
 16.48606;33.50954;30.14044;,
 21.48671;15.44168;0.00000;,
 7.65814;24.68163;0.00000;,
 29.64262;23.59759;-27.84610;,
 16.48606;33.50954;-30.14044;,
 49.33282;43.28778;-39.38034;,
 37.79857;54.82204;-42.62500;,
 63.22172;29.39887;-30.14044;,
 78.29193;44.46908;-21.31250;,
 84.53423;50.71137;0.00000;,
 78.29193;44.46908;21.31250;,
 63.22172;29.39887;30.14044;,
 48.15152;14.32865;21.31250;,
 41.90924;8.08636;0.00000;,
 48.15152;14.32865;-21.31250;,
 63.22172;29.39887;-30.14044;,
 72.40282;20.21775;-16.31186;,
 80.55876;28.37369;-11.53424;,
 83.93706;31.75198;0.00000;,
 80.55876;28.37369;11.53424;,
 72.40282;20.21775;16.31186;,
 64.24689;12.06182;11.53424;,
 60.86863;8.68353;0.00000;,
 64.24689;12.06182;-11.53424;,
 72.40282;20.21775;-16.31186;,
 74.41951;18.20106;-3.80249;,
 72.51828;16.29984;-2.68874;,
 74.69711;17.92346;0.00000;,
 76.32076;20.10229;-2.68874;,
 77.10829;20.88983;0.00000;,
 76.32076;20.10229;2.68874;,
 74.69711;17.92346;0.00000;,
 74.41951;18.20106;3.80249;,
 72.51828;16.29984;2.68874;,
 71.73074;15.51229;0.00000;,
 72.51828;16.29984;-2.68874;,
 74.41951;18.20106;-3.80249;,
 4.06849;21.09149;0.00000;,
 13.94786;30.97086;-33.73033;,
 13.94786;30.97086;33.73033;,
 37.79881;54.82180;-47.70187;,
 37.79881;54.82180;47.70187;,
 61.64975;78.67274;-33.73033;,
 61.64975;78.67274;33.73033;,
 71.52911;88.55212;0.00000;;
 
 192;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;12,10,36,37;,
 4;10,8,38,36;,
 4;39,38,8,6;,
 4;40,39,6,4;,
 4;41,40,4,1;,
 4;1,0,42,41;,
 4;0,43,44,42;,
 4;12,37,45,14;,
 3;46,45,37;,
 3;46,47,48;,
 3;49,46,48;,
 3;45,46,49;,
 3;37,36,46;,
 3;38,46,36;,
 3;38,39,46;,
 3;39,40,50;,
 4;51,52,53,54;,
 4;54,53,55,56;,
 4;56,55,57,58;,
 4;58,57,59,60;,
 4;60,59,61,62;,
 4;62,61,63,64;,
 4;64,63,65,66;,
 4;66,65,67,68;,
 4;52,69,70,53;,
 4;53,70,71,55;,
 4;55,71,72,57;,
 4;57,72,73,59;,
 4;59,73,74,61;,
 4;61,74,75,63;,
 4;63,75,76,65;,
 4;65,76,77,67;,
 4;69,78,79,70;,
 4;70,79,80,71;,
 4;71,80,81,72;,
 4;72,81,82,73;,
 4;73,82,83,74;,
 4;74,83,84,75;,
 4;75,84,85,76;,
 4;76,85,86,77;,
 4;64,87,88,62;,
 4;62,88,89,60;,
 4;90,58,60,89;,
 4;91,56,58,90;,
 4;92,54,56,91;,
 4;54,92,93,51;,
 4;51,93,94,95;,
 4;64,66,96,87;,
 3;97,87,96;,
 3;97,98,99;,
 3;100,98,97;,
 3;96,100,97;,
 3;87,97,88;,
 3;89,88,97;,
 3;89,97,90;,
 3;90,101,91;,
 4;102,103,104,105;,
 4;102,105,81,80;,
 4;102,80,79,106;,
 4;102,106,27,103;,
 4;105,104,107,108;,
 4;105,108,82,81;,
 4;106,79,78,109;,
 4;106,109,35,27;,
 4;108,107,110,111;,
 4;108,111,83,82;,
 4;109,78,85,112;,
 4;109,112,34,35;,
 4;113,32,33,114;,
 4;113,114,84,83;,
 4;112,85,84,114;,
 4;112,114,33,34;,
 4;115,116,117,118;,
 4;116,119,120,117;,
 4;119,121,122,120;,
 4;121,123,124,122;,
 4;123,125,126,124;,
 4;125,127,128,126;,
 4;127,129,130,128;,
 4;129,131,132,130;,
 4;118,117,133,134;,
 4;117,120,135,133;,
 4;120,122,136,135;,
 4;122,124,137,136;,
 4;124,126,138,137;,
 4;126,128,139,138;,
 4;128,130,140,139;,
 4;130,132,141,140;,
 4;134,133,142,143;,
 4;133,135,144,142;,
 4;135,136,145,144;,
 4;136,137,146,145;,
 4;137,138,147,146;,
 4;138,139,148,147;,
 4;139,140,149,148;,
 4;140,141,150,149;,
 3;151,152,153;,
 3;154,152,151;,
 3;155,152,154;,
 3;156,157,155;,
 3;158,157,156;,
 3;159,157,158;,
 3;160,161,157;,
 3;160,157,159;,
 4;149,150,162,161;,
 4;143,142,154,151;,
 4;144,145,156,155;,
 4;142,144,155,154;,
 4;145,146,158,156;,
 4;159,158,146,147;,
 4;147,148,160,159;,
 4;160,148,149,161;,
 4;163,164,165,166;,
 4;166,165,167,168;,
 4;168,167,169,170;,
 4;170,169,171,172;,
 4;172,171,173,174;,
 4;174,173,175,176;,
 4;176,175,177,178;,
 4;178,177,179,180;,
 4;164,181,182,165;,
 4;165,182,183,167;,
 4;167,183,184,169;,
 4;169,184,185,171;,
 4;171,185,186,173;,
 4;173,186,187,175;,
 4;175,187,188,177;,
 4;177,188,189,179;,
 4;181,190,191,182;,
 4;182,191,192,183;,
 4;183,192,193,184;,
 4;184,193,194,185;,
 4;185,194,195,186;,
 4;186,195,196,187;,
 4;187,196,197,188;,
 4;188,197,198,189;,
 3;199,200,201;,
 3;202,199,201;,
 3;203,202,201;,
 3;204,203,205;,
 3;206,204,205;,
 3;207,206,205;,
 3;208,205,209;,
 3;208,207,205;,
 4;197,209,210,198;,
 4;190,199,202,191;,
 4;192,203,204,193;,
 4;191,202,203,192;,
 4;193,204,206,194;,
 4;207,195,194,206;,
 4;195,207,208,196;,
 4;208,209,197,196;,
 4;211,176,178,212;,
 4;211,212,129,127;,
 4;211,127,125,213;,
 4;211,213,174,176;,
 4;212,178,180,214;,
 4;212,214,115,129;,
 4;213,125,123,215;,
 4;213,215,172,174;,
 4;214,180,166,216;,
 4;214,216,116,115;,
 4;215,123,121,217;,
 4;215,217,170,172;,
 4;216,166,168,218;,
 4;216,218,119,116;,
 4;217,121,119,218;,
 4;217,218,168,170;;
 
 MeshMaterialList {
  2;
  192;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
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
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\ParmFruit.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  212;
  -0.663175;-0.663654;-0.346067;,
  -0.836544;-0.490073;-0.244996;,
  -0.908295;-0.418330;-0.000004;,
  -0.836289;-0.490383;0.245245;,
  -0.663071;-0.663528;0.346507;,
  -0.490535;-0.836231;0.245140;,
  -0.418811;-0.908073;-0.000148;,
  -0.489878;-0.836651;-0.245017;,
  -0.544528;-0.544528;-0.637949;,
  -0.863503;-0.225553;-0.451098;,
  -0.995626;-0.093429;0.000000;,
  -0.863502;-0.225553;0.451098;,
  -0.544528;-0.544528;0.637949;,
  -0.225554;-0.863502;0.451098;,
  -0.093431;-0.995626;0.000000;,
  -0.225554;-0.863502;-0.451098;,
  -0.310177;-0.310176;-0.898655;,
  -0.759504;0.139151;-0.635445;,
  -0.945621;0.325269;0.000000;,
  -0.759504;0.139151;0.635445;,
  -0.310177;-0.310176;0.898655;,
  0.139151;-0.759504;0.635445;,
  0.325269;-0.945621;-0.000000;,
  0.139151;-0.759504;-0.635445;,
  -0.175306;-0.175306;-0.968780;,
  -0.659697;0.309084;-0.685031;,
  -0.860338;0.509725;0.000000;,
  -0.659697;0.309084;0.685031;,
  -0.175306;-0.175306;0.968780;,
  0.309085;-0.659696;0.685031;,
  0.509727;-0.860336;-0.000000;,
  0.309085;-0.659696;-0.685031;,
  -0.612418;-0.781118;0.121651;,
  -0.695680;-0.697780;0.170681;,
  -0.611226;-0.782195;-0.120717;,
  -0.695915;-0.698054;-0.168592;,
  -0.781441;-0.612426;-0.119517;,
  -0.816438;-0.577433;-0.000004;,
  -0.780590;-0.613304;0.120576;,
  -0.577867;-0.816131;-0.000146;,
  -0.706279;-0.707933;0.000825;,
  0.662988;0.663666;0.346403;,
  0.836544;0.490073;0.244996;,
  0.908295;0.418329;0.000004;,
  0.836290;0.490382;-0.245245;,
  0.663072;0.663528;-0.346507;,
  0.490200;0.836387;-0.245278;,
  0.418477;0.908227;0.000004;,
  0.489695;0.836659;0.245357;,
  0.544529;0.544528;0.637948;,
  0.863503;0.225553;0.451097;,
  0.995626;0.093430;0.000000;,
  0.863503;0.225553;-0.451097;,
  0.544529;0.544528;-0.637948;,
  0.225554;0.863502;-0.451098;,
  0.093431;0.995626;0.000000;,
  0.225554;0.863502;0.451098;,
  0.310177;0.310176;0.898655;,
  0.759504;-0.139151;0.635445;,
  0.945622;-0.325269;-0.000000;,
  0.759504;-0.139151;-0.635445;,
  0.310177;0.310176;-0.898655;,
  -0.139151;0.759504;-0.635445;,
  -0.325269;0.945622;0.000000;,
  -0.139151;0.759504;0.635445;,
  0.175306;0.175306;0.968781;,
  0.659696;-0.309085;0.685031;,
  0.860337;-0.509726;-0.000000;,
  0.659696;-0.309085;-0.685031;,
  0.175306;0.175306;-0.968781;,
  -0.309085;0.659696;-0.685031;,
  -0.509726;0.860337;0.000000;,
  -0.309085;0.659696;0.685031;,
  0.612124;0.781325;-0.121801;,
  0.695681;0.697779;-0.170683;,
  0.611082;0.782255;0.121065;,
  0.695775;0.698109;0.168942;,
  0.781443;0.612424;0.119518;,
  0.816439;0.577432;0.000004;,
  0.780590;0.613302;-0.120578;,
  0.577572;0.816340;0.000004;,
  0.706281;0.707931;-0.000825;,
  0.707099;0.707115;0.000000;,
  0.707102;0.707112;0.000008;,
  0.707102;0.707112;-0.000008;,
  0.707108;0.707106;0.000011;,
  0.707108;0.707106;-0.000011;,
  0.707113;0.707101;0.000008;,
  0.707113;0.707101;-0.000008;,
  0.707115;0.707099;0.000000;,
  0.707096;0.707118;0.000000;,
  0.707099;0.707114;0.000012;,
  0.707104;0.707110;0.000004;,
  0.707102;0.707111;0.000000;,
  0.707104;0.707110;-0.000004;,
  0.707099;0.707114;-0.000012;,
  0.707107;0.707106;0.000017;,
  0.707108;0.707106;0.000006;,
  0.707108;0.707106;-0.000006;,
  0.707107;0.707106;-0.000017;,
  0.707115;0.707099;0.000011;,
  0.707111;0.707103;0.000004;,
  0.707111;0.707103;-0.000004;,
  0.707115;0.707099;-0.000011;,
  0.707117;0.707096;0.000000;,
  0.707112;0.707102;0.000000;,
  0.137951;-0.137950;-0.980785;,
  0.628343;0.352442;-0.693520;,
  0.831471;0.555569;0.000000;,
  0.628343;0.352442;0.693520;,
  0.137951;-0.137950;0.980785;,
  -0.352443;-0.628343;0.693520;,
  -0.555570;-0.831470;0.000000;,
  -0.352443;-0.628343;-0.693520;,
  0.221545;-0.221545;-0.949650;,
  0.696370;0.253280;-0.671504;,
  0.893049;0.449959;0.000000;,
  0.696370;0.253280;0.671504;,
  0.221545;-0.221545;0.949650;,
  -0.253280;-0.696370;0.671504;,
  -0.449959;-0.893049;0.000000;,
  -0.253280;-0.696370;-0.671504;,
  0.417089;-0.417089;-0.807510;,
  0.820844;-0.013334;-0.570996;,
  0.988085;0.153907;0.000000;,
  0.820844;-0.013334;0.570996;,
  0.417089;-0.417089;0.807510;,
  0.013334;-0.820845;0.570996;,
  -0.153907;-0.988085;0.000000;,
  0.013334;-0.820845;-0.570996;,
  0.625769;-0.625769;-0.465646;,
  0.858593;-0.392945;-0.329262;,
  0.955031;-0.296506;-0.000000;,
  0.858593;-0.392945;0.329262;,
  0.625769;-0.625769;0.465646;,
  0.392945;-0.858592;0.329262;,
  0.296507;-0.955031;-0.000000;,
  0.392945;-0.858592;-0.329262;,
  0.707107;-0.707107;-0.000000;,
  0.616338;-0.779033;-0.115041;,
  0.697685;-0.697687;-0.162692;,
  0.779033;-0.616338;-0.115040;,
  0.812728;-0.582643;-0.000000;,
  0.779033;-0.616338;0.115040;,
  0.697685;-0.697687;0.162692;,
  0.616338;-0.779033;0.115041;,
  0.582645;-0.812727;-0.000000;,
  -0.137951;0.137951;0.980785;,
  -0.628344;-0.352441;0.693520;,
  -0.831471;-0.555568;0.000000;,
  -0.628344;-0.352441;-0.693520;,
  -0.137951;0.137951;-0.980785;,
  0.352443;0.628343;-0.693520;,
  0.555570;0.831470;0.000000;,
  0.352443;0.628343;0.693520;,
  -0.221545;0.221545;0.949650;,
  -0.696370;-0.253280;0.671504;,
  -0.893049;-0.449959;0.000000;,
  -0.696370;-0.253280;-0.671504;,
  -0.221545;0.221545;-0.949650;,
  0.253280;0.696370;-0.671504;,
  0.449959;0.893049;0.000000;,
  0.253280;0.696370;0.671504;,
  -0.417089;0.417089;0.807511;,
  -0.820844;0.013334;0.570996;,
  -0.988085;-0.153907;0.000000;,
  -0.820844;0.013334;-0.570996;,
  -0.417089;0.417089;-0.807511;,
  -0.013334;0.820845;-0.570996;,
  0.153907;0.988085;0.000000;,
  -0.013334;0.820845;0.570996;,
  -0.625768;0.625769;0.465647;,
  -0.858592;0.392945;0.329262;,
  -0.955031;0.296507;0.000000;,
  -0.858592;0.392945;-0.329262;,
  -0.625768;0.625769;-0.465647;,
  -0.392945;0.858592;-0.329262;,
  -0.296507;0.955031;0.000000;,
  -0.392945;0.858592;0.329262;,
  -0.707105;0.707108;-0.000000;,
  -0.616339;0.779032;0.115042;,
  -0.697685;0.697687;0.162693;,
  -0.779031;0.616341;0.115042;,
  -0.812726;0.582645;0.000000;,
  -0.779031;0.616341;-0.115042;,
  -0.697685;0.697687;-0.162693;,
  -0.616339;0.779032;-0.115042;,
  -0.582645;0.812727;0.000000;,
  -0.707183;0.707031;0.000000;,
  -0.707161;0.707053;-0.000076;,
  -0.707161;0.707053;0.000076;,
  -0.707107;0.707107;-0.000107;,
  -0.707107;0.707107;0.000107;,
  -0.707053;0.707160;-0.000075;,
  -0.707053;0.707160;0.000075;,
  -0.707031;0.707182;0.000000;,
  -0.707154;0.707059;0.000000;,
  -0.707140;0.707073;-0.000048;,
  -0.707181;0.707033;-0.000104;,
  -0.707212;0.707002;0.000000;,
  -0.707181;0.707033;0.000104;,
  -0.707140;0.707073;0.000048;,
  -0.707107;0.707107;-0.000068;,
  -0.707107;0.707107;-0.000147;,
  -0.707107;0.707107;0.000147;,
  -0.707107;0.707107;0.000068;,
  -0.707073;0.707140;-0.000048;,
  -0.707034;0.707180;-0.000103;,
  -0.707034;0.707180;0.000103;,
  -0.707073;0.707140;0.000048;,
  -0.707059;0.707154;0.000000;,
  -0.707003;0.707210;0.000000;;
  192;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;6,5,32,39;,
  4;5,4,33,32;,
  4;38,33,4,3;,
  4;37,38,3,2;,
  4;36,37,2,1;,
  4;1,0,35,36;,
  4;0,7,34,35;,
  4;6,39,34,7;,
  3;40,34,39;,
  3;40,37,36;,
  3;35,40,36;,
  3;34,40,35;,
  3;39,32,40;,
  3;33,40,32;,
  3;33,38,40;,
  3;38,37,40;,
  4;41,49,50,42;,
  4;42,50,51,43;,
  4;43,51,52,44;,
  4;44,52,53,45;,
  4;45,53,54,46;,
  4;46,54,55,47;,
  4;47,55,56,48;,
  4;48,56,49,41;,
  4;49,57,58,50;,
  4;50,58,59,51;,
  4;51,59,60,52;,
  4;52,60,61,53;,
  4;53,61,62,54;,
  4;54,62,63,55;,
  4;55,63,64,56;,
  4;56,64,57,49;,
  4;57,65,66,58;,
  4;58,66,67,59;,
  4;59,67,68,60;,
  4;60,68,69,61;,
  4;61,69,70,62;,
  4;62,70,71,63;,
  4;63,71,72,64;,
  4;64,72,65,57;,
  4;47,80,73,46;,
  4;46,73,74,45;,
  4;79,44,45,74;,
  4;78,43,44,79;,
  4;77,42,43,78;,
  4;42,77,76,41;,
  4;41,76,75,48;,
  4;47,48,75,80;,
  3;81,80,75;,
  3;81,77,78;,
  3;76,77,81;,
  3;75,76,81;,
  3;80,81,73;,
  3;74,73,81;,
  3;74,81,79;,
  3;79,81,78;,
  4;82,90,91,83;,
  4;82,83,92,93;,
  4;82,93,94,84;,
  4;82,84,95,90;,
  4;83,91,96,85;,
  4;83,85,97,92;,
  4;84,94,98,86;,
  4;84,86,99,95;,
  4;85,96,100,87;,
  4;85,87,101,97;,
  4;86,98,102,88;,
  4;86,88,103,99;,
  4;87,100,104,89;,
  4;87,89,105,101;,
  4;88,102,105,89;,
  4;88,89,104,103;,
  4;106,107,115,114;,
  4;107,108,116,115;,
  4;108,109,117,116;,
  4;109,110,118,117;,
  4;110,111,119,118;,
  4;111,112,120,119;,
  4;112,113,121,120;,
  4;113,106,114,121;,
  4;114,115,123,122;,
  4;115,116,124,123;,
  4;116,117,125,124;,
  4;117,118,126,125;,
  4;118,119,127,126;,
  4;119,120,128,127;,
  4;120,121,129,128;,
  4;121,114,122,129;,
  4;122,123,131,130;,
  4;123,124,132,131;,
  4;124,125,133,132;,
  4;125,126,134,133;,
  4;126,127,135,134;,
  4;127,128,136,135;,
  4;128,129,137,136;,
  4;129,122,130,137;,
  3;140,138,139;,
  3;141,138,140;,
  3;142,138,141;,
  3;143,138,142;,
  3;144,138,143;,
  3;145,138,144;,
  3;146,139,138;,
  3;146,138,145;,
  4;137,130,140,139;,
  4;130,131,141,140;,
  4;132,133,143,142;,
  4;131,132,142,141;,
  4;133,134,144,143;,
  4;145,144,134,135;,
  4;135,136,146,145;,
  4;146,136,137,139;,
  4;147,155,156,148;,
  4;148,156,157,149;,
  4;149,157,158,150;,
  4;150,158,159,151;,
  4;151,159,160,152;,
  4;152,160,161,153;,
  4;153,161,162,154;,
  4;154,162,155,147;,
  4;155,163,164,156;,
  4;156,164,165,157;,
  4;157,165,166,158;,
  4;158,166,167,159;,
  4;159,167,168,160;,
  4;160,168,169,161;,
  4;161,169,170,162;,
  4;162,170,163,155;,
  4;163,171,172,164;,
  4;164,172,173,165;,
  4;165,173,174,166;,
  4;166,174,175,167;,
  4;167,175,176,168;,
  4;168,176,177,169;,
  4;169,177,178,170;,
  4;170,178,171,163;,
  3;181,180,179;,
  3;182,181,179;,
  3;183,182,179;,
  3;184,183,179;,
  3;185,184,179;,
  3;186,185,179;,
  3;187,179,180;,
  3;187,186,179;,
  4;178,180,181,171;,
  4;171,181,182,172;,
  4;173,183,184,174;,
  4;172,182,183,173;,
  4;174,184,185,175;,
  4;186,176,175,185;,
  4;176,186,187,177;,
  4;187,180,178,177;,
  4;188,196,197,189;,
  4;188,189,198,199;,
  4;188,199,200,190;,
  4;188,190,201,196;,
  4;189,197,202,191;,
  4;189,191,203,198;,
  4;190,200,204,192;,
  4;190,192,205,201;,
  4;191,202,206,193;,
  4;191,193,207,203;,
  4;192,204,208,194;,
  4;192,194,209,205;,
  4;193,206,210,195;,
  4;193,195,211,207;,
  4;194,208,211,195;,
  4;194,195,210,209;;
 }
 MeshTextureCoords {
  219;
  0.000000;0.136230;,
  0.125000;0.136230;,
  0.125000;0.254240;,
  0.000000;0.254240;,
  0.250000;0.136230;,
  0.250000;0.254240;,
  0.375000;0.136230;,
  0.375000;0.254240;,
  0.500000;0.136230;,
  0.500000;0.254240;,
  0.625000;0.136230;,
  0.625000;0.254240;,
  0.750000;0.136230;,
  0.750000;0.254240;,
  0.875000;0.136230;,
  0.875000;0.254240;,
  1.000000;0.136230;,
  1.000000;0.254240;,
  0.125000;0.360780;,
  0.000000;0.360780;,
  0.250000;0.360780;,
  0.375000;0.360780;,
  0.500000;0.360780;,
  0.625000;0.360780;,
  0.750000;0.360780;,
  0.875000;0.360780;,
  1.000000;0.360780;,
  0.125000;0.448640;,
  0.000000;0.448640;,
  0.250000;0.448640;,
  0.375000;0.448640;,
  0.500000;0.448640;,
  0.625000;0.448640;,
  0.750000;0.448640;,
  0.875000;0.448640;,
  1.000000;0.448640;,
  0.625050;0.027560;,
  0.750110;0.027500;,
  0.499960;0.027560;,
  0.374950;0.027470;,
  0.249890;0.027480;,
  0.124890;0.027500;,
  -0.000040;0.027510;,
  -0.125000;0.136230;,
  -0.124890;0.027580;,
  0.875110;0.027580;,
  0.943700;0.000020;,
  1.249890;0.027480;,
  1.124890;0.027500;,
  0.999960;0.027510;,
  -0.056300;0.000020;,
  0.000000;0.136230;,
  0.000000;0.254240;,
  0.125000;0.254240;,
  0.125000;0.136230;,
  0.250000;0.254240;,
  0.250000;0.136230;,
  0.375000;0.254240;,
  0.375000;0.136230;,
  0.500000;0.254240;,
  0.500000;0.136230;,
  0.625000;0.254240;,
  0.625000;0.136230;,
  0.750000;0.254240;,
  0.750000;0.136230;,
  0.875000;0.254240;,
  0.875000;0.136230;,
  1.000000;0.254240;,
  1.000000;0.136230;,
  0.000000;0.360780;,
  0.125000;0.360780;,
  0.250000;0.360780;,
  0.375000;0.360780;,
  0.500000;0.360780;,
  0.625000;0.360780;,
  0.750000;0.360780;,
  0.875000;0.360780;,
  1.000000;0.360780;,
  0.000000;0.448640;,
  0.125000;0.448640;,
  0.250000;0.448640;,
  0.375000;0.448640;,
  0.500000;0.448640;,
  0.625000;0.448640;,
  0.750000;0.448640;,
  0.875000;0.448640;,
  1.000000;0.448640;,
  0.750110;0.027500;,
  0.625050;0.027560;,
  0.499960;0.027560;,
  0.374950;0.027470;,
  0.249890;0.027480;,
  0.124890;0.027500;,
  -0.000040;0.027510;,
  -0.124890;0.027580;,
  -0.125000;0.136230;,
  0.875110;0.027580;,
  0.943700;0.000020;,
  1.124890;0.027500;,
  1.249890;0.027480;,
  0.999960;0.027510;,
  -0.056300;0.000020;,
  0.125000;0.224320;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.224320;,
  0.125000;0.448640;,
  0.000000;0.000000;,
  0.250000;0.224320;,
  0.500000;0.448640;,
  0.000000;0.000000;,
  0.312500;0.224320;,
  0.875000;0.448640;,
  0.625000;0.448640;,
  0.750000;0.448640;,
  0.000000;0.448640;,
  0.125000;0.448640;,
  0.125000;0.562440;,
  0.000000;0.562440;,
  0.250000;0.448640;,
  0.250000;0.562440;,
  0.375000;0.448640;,
  0.375000;0.562440;,
  0.500000;0.448640;,
  0.500000;0.562440;,
  0.625000;0.448640;,
  0.625000;0.562440;,
  0.750000;0.448640;,
  0.750000;0.562440;,
  0.875000;0.448640;,
  0.875000;0.562440;,
  1.000000;0.448640;,
  1.000000;0.562440;,
  0.125000;0.724140;,
  0.000000;0.724140;,
  0.250000;0.724140;,
  0.375000;0.724140;,
  0.500000;0.724140;,
  0.625000;0.724140;,
  0.750000;0.724140;,
  0.875000;0.724140;,
  1.000000;0.724140;,
  0.125000;0.868940;,
  0.000000;0.868940;,
  0.250000;0.868940;,
  0.375000;0.868940;,
  0.500000;0.868940;,
  0.625000;0.868940;,
  0.750000;0.868940;,
  0.875000;0.868940;,
  1.000000;0.868940;,
  0.000000;0.969780;,
  -0.250000;1.000000;,
  -0.125000;0.969780;,
  0.125000;0.969780;,
  0.250000;0.969780;,
  0.375000;0.969780;,
  0.750000;1.000000;,
  0.500000;0.969780;,
  0.625000;0.969780;,
  0.750000;0.969780;,
  0.875000;0.969780;,
  1.000000;0.969780;,
  0.000000;0.448640;,
  0.000000;0.562440;,
  0.125000;0.562440;,
  0.125000;0.448640;,
  0.250000;0.562440;,
  0.250000;0.448640;,
  0.375000;0.562440;,
  0.375000;0.448640;,
  0.500000;0.562440;,
  0.500000;0.448640;,
  0.625000;0.562440;,
  0.625000;0.448640;,
  0.750000;0.562440;,
  0.750000;0.448640;,
  0.875000;0.562440;,
  0.875000;0.448640;,
  1.000000;0.562440;,
  1.000000;0.448640;,
  0.000000;0.724140;,
  0.125000;0.724140;,
  0.250000;0.724140;,
  0.375000;0.724140;,
  0.500000;0.724140;,
  0.625000;0.724140;,
  0.750000;0.724140;,
  0.875000;0.724140;,
  1.000000;0.724140;,
  0.000000;0.868940;,
  0.125000;0.868940;,
  0.250000;0.868940;,
  0.375000;0.868940;,
  0.500000;0.868940;,
  0.625000;0.868940;,
  0.750000;0.868940;,
  0.875000;0.868940;,
  1.000000;0.868940;,
  0.000000;0.969780;,
  -0.125000;0.969780;,
  -0.250000;1.000000;,
  0.125000;0.969780;,
  0.250000;0.969780;,
  0.375000;0.969780;,
  0.750000;1.000000;,
  0.500000;0.969780;,
  0.625000;0.969780;,
  0.750000;0.969780;,
  0.875000;0.969780;,
  1.000000;0.969780;,
  0.750000;0.448640;,
  0.875000;0.448640;,
  0.625000;0.448640;,
  0.500000;0.448640;,
  0.500000;0.448640;,
  0.125000;0.448640;,
  0.375000;0.448640;,
  0.250000;0.448640;;
 }
}
