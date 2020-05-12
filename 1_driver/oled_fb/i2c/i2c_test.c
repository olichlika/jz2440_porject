void i2c_test(void) {
    i2c_init();

    init_oled();
    
    OLED_Clear();

    //oled_draw_point(1, 1);
    //oled_show_char(1, 1, 'Q', 8);
    //oled_show_char(20, 1, 'W', 8);
    oled_show_string(0, 0, "can you help me to some water! thanks you!");
}