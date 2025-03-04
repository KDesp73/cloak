# Signing Process for Collaboration

> [!NOTE]
> This document outlines the key signing process when multiple team members need to collaborate securely using `cloak`.

## 1. Creating the Encryption Key

The first team member encrypts the project using `cloak`. If the encryption process is successful, `cloak.key` is generated. This key is:
- **Encrypted** using the member's RSA public key.
- **Signed** with their RSA private key to ensure authenticity.

Once encrypted, the `cloak.key`, along with all `*.cloak` encrypted files and non-sensitive files, is committed and pushed to a shared remote repository. This allows all team members to access and sign the key while maintaining security.

## 2. Signing the Key

After pulling the latest changes from the repository, all team members must individually sign `cloak.key` using `cloak`. This ensures that multiple trusted signatures validate the key. The process is as follows:

1. Each team member verifies the integrity of the `cloak.key` by decrypting and checking the original signature.
2. They sign `cloak.key` using their own RSA private key.
3. The newly generated signature is appended to `cloak.sig`, which contains multiple trusted signatures.
4. The updated signatures are committed and pushed to the repository.

This process prevents unauthorized modifications and ensures that all team members agree on the encrypted data.

## 3. Decryption & Signature Validation

Each time decryption is performed, `cloak` automatically verifies `cloak.sig` to ensure that:
- The AES key was originally signed by an authorized member.
- The key has been signed by all required team members.
- No tampering has occurred.

If all checks pass, the decryption proceeds. If any validation fails, `cloak` will reject the decryption attempt, alerting the user to a potential security issue.

## 4. Continuous Integration (CI) Validation

To integrate with CI/CD pipelines, the trusted public key list should be stored in the repository. The CI process can then:
1. **Validate Signatures**: Automatically verify that `cloak.sig` contains valid signatures from authorized team members.
2. **Ensure Compliance**: Enforce that the key has been signed by a minimum number of team members before allowing deployment.
3. **Prevent Unauthorized Changes**: Block merges if `cloak.sig` validation fails, preventing unauthorized modifications to encrypted data.

By enforcing these steps, teams can securely collaborate without relying on a centralized key server while maintaining strong security guarantees.

