
uint32_t steps_per_second(float angular_speed, int reduction, int microstepping, int max_angular_speed, int steps_per_rev = 200) {
  if (angular_speed < 0)
    return 0;
  if (angular_speed > max_angular_speed)
    angular_speed = max_angular_speed;

    uint32_t steps_per_second = (angular_speed / 360.0) * reduction * microstepping * steps_per_rev;
    return steps_per_second;
  
}