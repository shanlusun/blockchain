**MSP(Membership Service Provider)**

MSP 在[官方文档介绍](http://hyperledger-fabric.readthedocs.io/en/latest/glossary.html#membership-service-provider "官方文档介绍")的并不多，简单理解MSP为Fabric网络的成员：client，peers等提供证书。Client用证书来鉴定Transaction，Peers（实际为Endorsing peers）用证书来鉴定Transaction进而做Endorsement。

**对MSP的理解，我们需要回答下面几个关键问题：**
1. MSP在一个Transaction Flow中什么阶段起作用？怎么起作用？
2. MSP对证书做鉴定时，是否与Fabric-CA有交互？如果没有，如何完成对证书的鉴定？MSP是否会检查一个证书有没有被回收（revocation）？
3. 联盟链会有新成员加入，此时MSP如何完成动态更新进而认证新的证书？或者说Fabric 官方wiki上面提到的Dynamic MSP是如何实现的？
----------------------
>第一个问题：MSP在一个Transaction Flow中什么阶段起作用？怎么起作用？
>>**回答**：MSP主要在Endorsing Peer 做Endorsement以及在Ordering Service上会被调用，对证书进行鉴定。
至于怎么起作用，下面用实际code来举出一个例子说明Membership Service实际应用场景：
在做endorser时，在我们之前画的一个[Transaction Flow](https://github.com/shanlusun/blockchain/blob/master/hyperledger-fabric/TransactionFlow/Fabric-Transaction%20Flow.png "TX Flow")的流程图里面提到，Endorseing Peers 会 Verify signature，主要就是依赖MSP来完成。
----------------------
github.com\hyperledger\fabric\core\endorser\endorser.go line 372： **prop, hdr, hdrExt, err := validation.ValidateProposalMessage(signedProp)**
```go
// ProcessProposal process the Proposal
func (e *Endorser) ProcessProposal(ctx context.Context, signedProp *pb.SignedProposal) (*pb.ProposalResponse, error) {
       // at first, we check whether the message is valid
       prop, hdr, hdrExt, err := validation.ValidateProposalMessage(signedProp)
       if err != nil {
              return &pb.ProposalResponse{Response: &pb.Response{Status: 500, Message: err.Error()}}, err
       }
 
       //...
 
       return pResp, nil
}
```
----------------------
src\github.com\hyperledger\fabric\core\common\validation\msgvalidation.go line 100： **err = checkSignatureFromCreator(shdr.Creator, signedProp.Signature, signedProp.ProposalBytes, chdr.ChannelId)**
```go
// ValidateProposalMessage checks the validity of a SignedProposal message
// this function returns Header and ChaincodeHeaderExtension messages since they
// have been unmarshalled and validated
func ValidateProposalMessage(signedProp *pb.SignedProposal) (*pb.Proposal, *common.Header, *pb.ChaincodeHeaderExtension, error) {
       putilsLogger.Debugf("ValidateProposalMessage starts for signed proposal %p", signedProp)
 
       // extract the Proposal message from signedProp
       prop, err := utils.GetProposal(signedProp.ProposalBytes)
       if err != nil {
              return nil, nil, nil, err
       }
 
       // 1) look at the ProposalHeader
       hdr, err := utils.GetHeader(prop.Header)
       if err != nil {
              return nil, nil, nil, err
       }
 
       // validate the header
       chdr, shdr, err := validateCommonHeader(hdr)
       if err != nil {
              return nil, nil, nil, err
       }
 
       // validate the signature
       err = checkSignatureFromCreator(shdr.Creator, signedProp.Signature, signedProp.ProposalBytes, chdr.ChannelId)
       if err != nil {
              return nil, nil, nil, err
       }
       //...
}
```
----------------------
src\github.com\hyperledger\fabric\core\common\validation\msgvalidation.go line 163 : **err = creator.Validate()**
```go
// given a creator, a message and a signature,
// this function returns nil if the creator
// is a valid cert and the signature is valid
func checkSignatureFromCreator(creatorBytes []byte, sig []byte, msg []byte, ChainID string) error {
       putilsLogger.Debugf("checkSignatureFromCreator starts")
 
       // check for nil argument
       if creatorBytes == nil || sig == nil || msg == nil {
              return errors.New("Nil arguments")
       }
 
       mspObj := mspmgmt.GetIdentityDeserializer(ChainID)
       if mspObj == nil {
              return fmt.Errorf("could not get msp for chain [%s]", ChainID)
       }
 
       // get the identity of the creator
       creator, err := mspObj.DeserializeIdentity(creatorBytes)
       if err != nil {
              return fmt.Errorf("Failed to deserialize creator identity, err %s", err)
       }
 
       putilsLogger.Debugf("checkSignatureFromCreator info: creator is %s", creator.GetIdentifier())
 
       // ensure that creator is a valid certificate
       err = creator.Validate()
       if err != nil {
              return fmt.Errorf("The creator certificate is not valid, err %s", err)
       }
       //...
       return nil
}
```
----------------------
src\github.com\hyperledger\fabric\msp\msp.go 中定义的关键 interface为 line 126: **Validate() error**
```go
// Identity interface defining operations associated to a "certificate".
// That is, the public part of the identity could be thought to be a certificate,
// and offers solely signature verification capabilities. This is to be used
// at the peer side when verifying certificates that transactions are signed
// with, and verifying signatures that correspond to these certificates.///
type Identity interface {
 
       // GetIdentifier returns the identifier of that identity
       GetIdentifier() *IdentityIdentifier
 
       // GetMSPIdentifier returns the MSP Id for this instance
       GetMSPIdentifier() string
 
       // Validate uses the rules that govern this identity to validate it.
       // E.g., if it is a fabric TCert implemented as identity, validate
       // will check the TCert signature against the assumed root certificate
       // authority.
       Validate() error
 
       // GetOrganizationalUnits returns zero or more organization units or
       // divisions this identity is related to as long as this is public
       // information. Certain MSP implementations may use attributes
       // that are publicly associated to this identity, or the identifier of
       // the root certificate authority that has provided signatures on this
       // certificate.
       // Examples:
       //  - if the identity is an x.509 certificate, this function returns one
       //    or more string which is encoded in the Subject's Distinguished Name
       //    of the type OU
       // TODO: For X.509 based identities, check if we need a dedicated type
       //       for OU where the Certificate OU is properly namespaced by the
       //       signer's identity
       GetOrganizationalUnits() []msp.FabricOUIdentifier
 
       // Verify a signature over some message using this identity as reference
       Verify(msg []byte, sig []byte) error
 
       // VerifyOpts a signature over some message using this identity as reference
       VerifyOpts(msg []byte, sig []byte, opts SignatureOpts) error
 
       // VerifyAttributes verifies attributes given a proof
       VerifyAttributes(proof []byte, spec *AttributeProofSpec) error
 
       // Serialize converts an identity to bytes
       Serialize() ([]byte, error)
 
       // SatisfiesPrincipal checks whether this instance matches
       // the description supplied in MSPPrincipal. The check may
       // involve a byte-by-byte comparison (if the principal is
       // a serialized identity) or may require MSP validation
       SatisfiesPrincipal(principal *msp.MSPPrincipal) error
}
```
这些interface 的实际实现在 src\github.com\hyperledger\fabric\msp\mspimpl.go可以找到。
在peer中也有对MSP的调用，比如 github.com\hyperledger\fabric\peer\gossip\mcs\mcs.go line 252： **func (s *mspMessageCryptoService) getValidatedIdentity(peerIdentity api.PeerIdentityType) (msp.Identity, common.ChainID, error)** ，不赘述。


>第二个问题： MSP对证书做鉴定时，是否与Fabric-CA有交互？如果没有，如何完成对证书的鉴定？MSP是否会检查一个证书有没有被回收（revocation）？
>>**回答**： 目前从code来看MSP并没有与Fabric-CA交互，但不排除之后会增加这一交互。
code的注释中指出以后对证书的鉴定会增加更多严格的规则，判断依据：src\github.com\hyperledger\fabric\msp\mspimpl.go line 701开始**注释部分**
```go
// getCertificationChainForBCCSPIdentity returns the certification chain of the passed bccsp identity within this msp
func (msp *bccspmsp) getCertificationChainForBCCSPIdentity(id *identity) ([]*x509.Certificate, error) {
       if id == nil {
              return nil, errors.New("Invalid bccsp identity. Must be different from nil.")
       }
 
       // we expect to have a valid VerifyOptions instance
       if msp.opts == nil {
              return nil, errors.New("Invalid msp instance")
       }
 
       // CAs cannot be directly used as identities..
       if id.cert.IsCA {
              return nil, errors.New("A CA certificate cannot be used directly by this MSP")
       }
 
       // at this point we might want to perform some
       // more elaborate validation. We do not do this
       // yet because we do not want to impose any
       // constraints without knowing the exact requirements,
       // but we at least list the kind of extra validation that we might perform:
       // 1) we might only allow a single verification chain (e.g. we expect the
       //    cert to be signed exactly only by the CA or only by the intermediate)
       // 2) we might want to let golang find any path, and then have a blacklist
       //    of paths (e.g. it can be signed by CA -> iCA1 -> iCA2 and it can be
       //    signed by CA but not by CA -> iCA1)
 
       // ask golang to validate the cert for us based on the options that we've built at setup time
       validationChain, err := id.cert.Verify(*(msp.opts))
       if err != nil {
              return nil, fmt.Errorf("The supplied identity is not valid, Verify() returned %s", err)
       }
 
       // we only support a single validation chain;
       // if there's more than one then there might
       // be unclarity about who owns the identity
       if len(validationChain) != 1 {
              return nil, fmt.Errorf("This MSP only supports a single validation chain, got %d", len(validationChain))
       }
 
       // we expect a chain of length at least 2
       if len(validationChain[0]) < 2 {
              return nil, fmt.Errorf("Expected a chain of length at least 2, got %d", len(validationChain))
       }
 
       return validationChain[0], nil
}
```
MSP目前是靠初始化时，预先载入待验证节点所分配的证书，构成一个Cert-chain来进行鉴定。
详情可以参考code细节：src\crypto\x509\verify.go 
```go
// Verify attempts to verify c by building one or more chains from c to a
// certificate in opts.Roots, using certificates in opts.Intermediates if
// needed. If successful, it returns one or more chains where the first
// element of the chain is c and the last element is from opts.Roots.
//
// If opts.Roots is nil and system roots are unavailable the returned error
// will be of type SystemRootsError.
//
// WARNING: this doesn't do any revocation checking.
func (c *Certificate) Verify(opts VerifyOptions) (chains [][]*Certificate, err error) {
       // Platform-specific verification needs the ASN.1 contents so
       // this makes the behavior consistent across platforms.
       if len(c.Raw) == 0 {
              return nil, errNotParsed
       }
       if opts.Intermediates != nil {
              for _, intermediate := range opts.Intermediates.certs {
                     if len(intermediate.Raw) == 0 {
                            return nil, errNotParsed
                     }
              }
       }
 
       // Use Windows's own verification and chain building.
       if opts.Roots == nil && runtime.GOOS == "windows" {
              return c.systemVerify(&opts)
       }
 
       if len(c.UnhandledCriticalExtensions) > 0 {
              return nil, UnhandledCriticalExtension{}
       }
 
       if opts.Roots == nil {
              opts.Roots = systemRootsPool()
              if opts.Roots == nil {
                     return nil, SystemRootsError{systemRootsErr}
              }
       }
 
       err = c.isValid(leafCertificate, nil, &opts)
       if err != nil {
              return
       }
 
       if len(opts.DNSName) > 0 {
              err = c.VerifyHostname(opts.DNSName)
              if err != nil {
                     return
              }
       }
 
       var candidateChains [][]*Certificate
       if opts.Roots.contains(c) {
              candidateChains = append(candidateChains, []*Certificate{c})
       } else {
              if candidateChains, err = c.buildChains(make(map[int][][]*Certificate), []*Certificate{c}, &opts); err != nil {
                     return nil, err
              }
       }
 
       keyUsages := opts.KeyUsages
       if len(keyUsages) == 0 {
              keyUsages = []ExtKeyUsage{ExtKeyUsageServerAuth}
       }
 
       // If any key usage is acceptable then we're done.
       for _, usage := range keyUsages {
              if usage == ExtKeyUsageAny {
                     chains = candidateChains
                     return
              }
       }
 
       for _, candidate := range candidateChains {
              if checkChainForKeyUsage(candidate, keyUsages) {
                     chains = append(chains, candidate)
              }
       }
 
       if len(chains) == 0 {
              err = CertificateInvalidError{c, IncompatibleUsage}
       }
 
       return
}
```

>第三个问题：联盟链会有新成员加入，此时MSP如何完成动态更新进而认证新的证书？或者说Fabric 官方wiki上面提到的Dynamic MSP是如何实现的？
>>**回答**： 目前MSP不支持动态local MSP的配置进而认证新的证书。从实际code和资料来看Dynamic MSP并没有实现。
参考以下细节：
参考目前官方JIRA提交的ticket：[“As an infrastructure developer I want to be able to automate Local MSP update”](https://jira.hyperledger.org/browse/FAB-2072 "ticket")，已经提出需要“动态更新local MSP”的需求。
Note：local MSP 是peer和orderer在本地config的一些MSP认证文件，包含CA和数字签名。(可以参考实例e2e_cli)
