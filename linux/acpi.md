- acpi init process
```
start_kernel()
=> setup_arch
    => acpi_boot_table_init
        => acpi_table_init
            => acpi_initialize_tables
                => acpi_os_get_root_pointer //Get the address of the RSDP
                => acpi_tb_parse_root_table
                    => acpi_tb_print_table_header
                    => acpi_tb_install_standard_table
    => early_acpi_boot_init
=> acpi_early_init
```