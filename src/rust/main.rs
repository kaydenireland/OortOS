#![no_std]

unsafe extern "C" {
    fn puts(str: *const u8);
}

#[unsafe(no_mangle)]
pub extern "C" fn rust_main() {
    unsafe {
        puts(b"Hello from Rust!\0".as_ptr());
    }
}