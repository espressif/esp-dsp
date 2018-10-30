uint8_t is_power_of_two(int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}