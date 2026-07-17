# TODO

## High Priority
- [ ] Push v2.5.0 to Chocolatey — troubleshoot 403 Forbidden on `push.chocolatey.org`
- [ ] Update `choco_api_key.txt` with working API key once resolved
- [ ] Commit remaining changes after Chocolatey push succeeds

## Medium Priority
- [ ] Update Winget PR if Chocolatey changes affect it
- [ ] Verify website download links point to correct GitHub Release assets
- [ ] Test `choco install emtypyie-cli` end-to-end after package is approved

## Low Priority / Future
- [ ] Automate Chocolatey push in `release.yml` (currently manual due to 403)
- [ ] Add Chocolatey badge to README once package is live
- [ ] Consider signing up at https://community.chocolatey.org as `myrachane` if `emtypyie` account push issue persists
- [ ] **Robust error handling pass:**
  - Add input validation (bounds checking on all string buffers in C)
  - Replace manual JSON parsing with cJSON library (already in lib/)
  - Surface WinHTTP error details (DNS/timeout/TLS) instead of silent 0
  - Add retry logic (3 attempts with backoff) for network operations
  - Verify downloaded files against checksums
  - Wrap all Node.js `execSync` calls in try-catch
  - Clean up partial files on install failure (rollback)
  - Add unit tests for error paths and failure modes
