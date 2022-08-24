//lamp A                                   R                                         G                                             B                                        w
byte lamp1[21][4][8]  = {{{  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 100,   0, 100, 255,   0, 255,   0}, {255,   0, 255,   0,   0, 100,   0, 100}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //1
                        {{255,   0,   0,   0,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0, 255}, {255,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //2
                        {{  0,   0,   0,   0, 255,   0,   0, 255}, {255,   0,   0, 255, 255,   0,   0,   0}, {  0,   0,   0, 255,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0, 255,   0, 255,   0}, {255,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0, 255,   0,   0,   0, 255,   0}, {255,   0,   0,   0, 255,   0,   0,   0}, {255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //5
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0, 255,   0,   0,   0, 255,   0,   0}, {  0,   0,   0, 255,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255,   0,   0, 255, 255,   0,   0, 255}, {  0, 255,   0,   0,   0, 255,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0,   0, 255}, {  0, 255, 255, 255,   0, 255,   0,   0}, {  0, 255,   0,   0, 255, 255, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0,   0, 255, 255,   0}, {  0, 255,   0,   0, 255, 255,   0, 255}, {  0,   0, 255, 255, 255,   0, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //9
                        {{255,   0, 255,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}},  //10
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {255,   0, 255,   0, 255,   0,   0,   0}, {255,   0, 255,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}},  //11
                        {{  0,   0, 255, 255,   0,   0, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0, 255, 255,   0,   0, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0,   0, 255,   0,   0,   0, 255}, {  0, 255,   0, 255,   0, 255,   0, 255}, {255, 255, 255,   0, 255, 255, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}, {255,   0, 255,   0, 255,   0, 255,   0}},  //14
                        {{255,   0,   0,   0,   0,   0, 255,   0}, {  0,   0,   0,   0, 255,   0,   0,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255, 255, 255,   0, 255, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0, 255,   0,   0, 255, 255,   0,   0}, {255,   0,   0,   0, 255,   0,   0, 255}, {255,   0,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}}, //17
                        {{  0, 255, 255, 255,   0, 255, 255, 255}, {255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 100,   0,   0,   0,   0,   0,   0}, {  0,   0, 255, 100,   0,   0,   0,   0}, {  0,   0,   0,   0, 255, 100,   0,   0}, {  0,   0,   0,   0,   0,   0, 255, 100}}, //19
                        {{255, 100,   0,   0,   0,   0,   0,   0}, {  0,   0, 255, 100,   0,   0,   0,   0}, {  0,   0,   0,   0, 255, 100,   0,   0}, {  0,   0,   0,   0,   0,   0, 255, 100}}, //20
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {255,   0,   0,   0,   0,   0,   0,   0}, {255,   0,   0,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0, 255}}}; //21

//lamp B
byte lamp2[21][4][8]  = {{{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0, 255,   0, 255}, {  0, 255,   0, 255,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //1
                        {{  0, 255,   0,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}, {  0, 255,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //2
                        {{  0, 255, 255,   0, 255,   0, 255, 255}, {255, 255,   0, 255, 255,   0,   0,   0}, {  0,   0,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0, 255,   0, 255,   0}, {255,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //5
                        {{  0,   0,   0, 255,   0,   0,   0, 255}, {255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255,   0,   0, 255, 255,   0,   0, 255}, {  0, 255,   0,   0,   0, 255,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 255,   0, 255,   0, 255,   0,   0}, {  0,   0, 255, 255, 255,   0, 255,   0}, {255,   0, 255,   0,   0, 255, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0, 255,   0, 255,   0, 255,   0, 255}, {  0,   0, 255,   0, 255,   0, 255,   0}, {255,   0, 255,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //9
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0, 255,   0, 255,   0, 255,   0}, {255,   0,   0,   0,   0,   0,   0,   0}},  //10
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {255,   0, 255,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}},  //11
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255, 255,   0,   0, 255, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255,   0, 255,   0, 255,   0, 255,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}},  //14
                        {{  0,   0,   0, 255,   0, 255,   0,   0}, {  0, 255,   0, 255,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255, 255, 255,   0, 255, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0, 255,   0,   0, 255, 255,   0,   0}, {255,   0,   0,   0, 255,   0,   0, 255}, {255,   0,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}}, //17
                        {{  0, 255, 255, 255,   0, 255, 255, 255}, {255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0,   0,   0,   0,   0}, {  0,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}}, //19
                        {{255, 100,   0,   0,   0,   0,   0,   0}, {  0,   0, 255, 100,   0,   0,   0,   0}, {  0,   0,   0,   0, 255, 100,   0,   0}, {  0,   0,   0,   0,   0,   0, 255, 100}}, //20
                        {{  0,   0,   0,   0,   0, 255,   0,   0}, {255, 255,   0,   0,   0,   0,   0,   0}, {255,   0,   0,   0,   0, 255, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}}}; //21
//lamp C
byte lamp3[21][4][8]  = {{{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0, 255,   0, 255}, {  0, 255,   0, 255,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //1
                        {{  0,   0, 255,   0,   0, 255,   0,   0}, {  0,   0,   0,   0,   0, 255,   0,   0}, {  0,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //2
                        {{  0, 255, 255,   0, 255,   0, 255, 255}, {255, 255,   0, 255, 255,   0,   0,   0}, {  0,   0,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0, 255,   0, 255,   0}, {255,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //5
                        {{  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0,   0, 255,   0,   0,   0, 255}, {  0, 255,   0,   0,   0, 255,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255,   0,   0, 255, 255,   0,   0, 255}, {  0, 255,   0,   0,   0, 255,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0, 255, 255,   0, 255,   0, 255,   0}, {  0,   0,   0, 255, 255, 255,   0, 255}, {255, 255,   0, 255,   0,   0, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0, 255,   0,   0, 255,   0,   0}, {255,   0,   0, 255, 255,   0, 255, 255}, {  0, 255,   0,   0, 255,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //9
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0, 255,   0, 255,   0, 255,   0}, {255,   0,   0,   0,   0,   0,   0,   0}},  //10
                        {{  0,   0, 255,   0,   0,   0,   0,   0}, {255,   0, 255,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}},  //11
                        {{255, 255,   0,   0, 255, 255,   0,   0}, {255, 255,   0,   0, 255, 255,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255,   0, 255,   0, 255,   0, 255,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}},  //14
                        {{  0,   0,   0, 255,   0, 255,   0,   0}, {  0, 255,   0, 255,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255, 255, 255,   0, 255, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0, 255,   0,   0, 255, 255,   0,   0}, {255,   0,   0,   0, 255,   0,   0, 255}, {255,   0,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}}, //17
                        {{  0, 255, 255, 255,   0, 255, 255, 255}, {255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0,   0,   0,   0,   0}, {  0,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}}, //19
                        {{  0,   0, 255,   0,   0,   0, 255,   0}, {  0, 255,   0, 255,   0,   0,   0,   0}, {255,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0, 255, 100,   0}}, //20
                        {{  0,   0, 255,   0, 255, 255,   0,   0}, {  0,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0, 255,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}}}; //21
//lamp D                                   R                                         G                                             B                                        w
byte lamp4[21][4][8]  = {{{  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 100,   0, 100, 255,   0, 255,   0}, {255,   0, 255,   0,   0, 100,   0, 100}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //1
                        {{  0,   0,   0, 255, 255,   0,   0,   0}, {  0,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0, 255,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //2
                        {{  0,   0,   0,   0, 255,   0,   0, 255}, {255,   0,   0, 255, 255,   0,   0,   0}, {  0,   0,   0, 255,   0,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0, 255,   0, 255,   0}, {255,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0, 255,   0,   0,   0, 255,   0}, {255,   0,   0,   0, 255,   0,   0,   0}, {255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //5
                        {{  0, 255,   0,   0,   0, 255,   0,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {255,   0,   0,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255,   0,   0, 255, 255,   0,   0, 255}, {  0, 255,   0,   0,   0, 255,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0, 255, 255,   0, 255,   0, 255}, {255,   0,   0,   0, 255, 255, 255,   0}, {255, 255, 255,   0, 255,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255, 255, 255, 255,   0,   0,   0, 255}, {  0,   0,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},  //9
                        {{255,   0, 255,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}},  //10
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {255,   0, 255,   0, 255,   0,   0,   0}, {255,   0, 255,   0, 255,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255,   0}},  //11
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {255,   0,   0, 255, 255,   0,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0,   0, 255,   0,   0,   0, 255}, {  0, 255,   0, 255,   0, 255,   0, 255}, {255, 255, 255,   0, 255, 255, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}, {255,   0, 255,   0, 255,   0, 255,   0}},  //14
                        {{255,   0,   0,   0,   0,   0, 255,   0}, {  0,   0,   0,   0, 255,   0,   0,   0}, {  0,   0, 255,   0,   0,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255, 255, 255,   0, 255, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{  0, 255,   0,   0, 255, 255,   0,   0}, {255,   0,   0,   0, 255,   0,   0, 255}, {255,   0,   0, 255,   0, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}}, //17
                        {{  0, 255, 255, 255,   0, 255, 255, 255}, {255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 100,   0,   0,   0,   0,   0,   0}, {  0,   0, 255, 100,   0,   0,   0,   0}, {  0,   0,   0,   0, 255, 100,   0,   0}, {  0,   0,   0,   0,   0,   0, 255, 100}}, //19
                        {{  0,   0,   0, 255,   0,   0,   0, 255}, {  0,   0, 255,   0, 255,   0,   0,   0}, {  0, 255,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0, 255, 100}}, //20
                        {{  0,   0, 255,   0, 255,   0,   0,   0}, {  0,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0,   0,   0, 255,   0,   0,   0,   0}}}; //21

//Panel                                      R                                         G                                             B                                        w
byte panel[21][3][8]  = {{{  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255,   0, 255, 255,   0, 255,   0}, {255,   0, 255,   0,   0, 255,   0, 255}},  //1
                        {{255, 255, 255, 255, 255, 255, 255, 255}, {  0,   0,   0,   0, 255, 255, 255, 255}, {255, 255, 255, 255,   0,   0,   0,   0}},  //2
                        {{  0, 255, 255,   0, 255,   0, 255, 255}, {255, 255,   0, 255, 255,   0,   0,   0}, {  0,   0,   0, 255,   0, 255,   0, 255}},
                        {{255, 255,   0,   0,   0,   0,   0,   0}, {  0,   0,   0,   0, 255, 255, 255, 255}, {  0,   0, 255, 255,   0,   0, 255, 255}},
                        {{  0, 255, 255,   0,   0, 255, 255,   0}, {255,   0,   0, 255, 255,   0,   0, 255}, {255,   0,   0, 255, 255,   0,   0, 255}},  //5
                        {{255,   0,   0,   0, 255,   0,   0,   0}, {  0, 255, 255,   0,   0, 255,   0,   0}, {  0,   0,   0, 255,   0,   0, 255, 255}},
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {255,   0,   0, 255, 255,   0,   0, 255}, {  0, 255,   0,   0,   0, 255,   0,   0}},
                        {{255,   0, 255,   0, 255,   0,   0, 255}, {  0, 255, 255, 255,   0, 255,   0,   0}, {  0, 255,   0,   0, 255, 255, 255, 255}},
                        {{255,   0,   0,   0,   0, 255, 255,   0}, {  0, 255,   0,   0, 255, 255,   0, 255}, {  0,   0, 255, 255, 255,   0, 255, 255}},  //9
                        {{255,   0, 255,   0, 255,   0, 255,   0}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}},  //10
                        {{  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}, {255,   0, 255,   0, 255,   0, 255,   0}},  //11
                        {{  0,   0, 255, 255,   0,   0, 255, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {255, 255,   0,   0, 255, 255,   0,   0}},
                        {{  0,   0,   0, 255,   0,   0,   0, 255}, {  0, 255,   0, 255,   0, 255,   0, 255}, {255, 255, 255,   0, 255, 255, 255,   0}},
                        {{255,   0, 255,   0,   0,   0,   0,   0}, {  0,   0,   0,   0, 255,   0, 255,   0}, {  0, 255,   0, 255,   0, 255,   0, 255}},  //14
                        {{255,   0,   0,   0,   0,   0, 255, 255}, {  0, 255, 255, 255, 255,   0,   0,   0}, {  0,   0, 255,   0,   0, 255, 255,   0}},
                        {{255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}, {  0, 255, 255, 255,   0, 255, 255, 255}},
                        {{  0, 255,   0,   0, 255, 255,   0,   0}, {255,   0, 255,   0, 255,   0, 255, 255}, {255,   0,   0, 255,   0, 255,   0, 255}}, //17
                        {{  0, 255, 255, 255,   0, 255, 255, 255}, {255, 255,   0, 255, 255, 255,   0, 255}, {  0,   0,   0,   0,   0,   0,   0,   0}},
                        {{255, 255, 255,   0,   0,   0,   0,   0}, {  0,   0, 255, 255, 255,   0,   0,   0}, {  0,   0,   0,   0,   0, 255, 255, 255}}, //19
                        {{255, 255, 255,   0,   0,   0,   0,   0}, {  0,   0, 255, 255, 255,   0,   0,   0}, {  0,   0,   0,   0,   0, 255, 255, 255}}, //20
                        {{  0,   0, 255,   0, 255, 255,   0,   0}, {255,   0, 255,   0,   0,   0,   0, 255}, {255, 255,   0,   0,   0,   0, 255,   0}}}; //21
