void i2c_test(void) {
    i2c_init();

    init_oled();
    
    OLED_Clear();

    oled_draw_point(1, 1);
}