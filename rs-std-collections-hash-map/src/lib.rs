use rs_token::*;

fn make_keyword_map() -> std::collections::HashMap<&'static [u8], TokenType> {
    let mut result: std::collections::HashMap<&'static [u8], TokenType> = std::collections::HashMap::new();
    result.reserve(KEYWORD_TOKENS.len());
    for kt in KEYWORD_TOKENS {
        result.insert(kt.keyword, kt.type_);
    }
    result
}

static mut KEYWORDS: std::mem::MaybeUninit<std::collections::HashMap<&'static [u8], TokenType>> = std::mem::MaybeUninit::uninit();

#[link_section = ".init_array"]
#[used]
static INIT_KEYWORDS: unsafe extern "C" fn() = {
    unsafe extern "C" fn init() {
        KEYWORDS.write(make_keyword_map());
    }
    init
};

type c_size_t = std::ffi::c_ulong;

#[no_mangle]
pub unsafe extern "C" fn look_up_identifier(
    identifier: *const std::ffi::c_char,
    size: c_size_t,
) -> TokenType {
    let identifier: &[u8] = std::slice::from_raw_parts(identifier.cast::<u8>(), size as usize);
    *KEYWORDS.assume_init_ref().get(identifier).unwrap_or(&TokenType::identifier)
}
