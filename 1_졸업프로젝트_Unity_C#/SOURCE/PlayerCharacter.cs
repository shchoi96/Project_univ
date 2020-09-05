using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Photon.Pun;
using Photon.Realtime;
using UnityEngine.UI;
using System.Reflection;
using System;

public class PlayerCharacter : LivingEntity, IPunObservable
{
    // 플레이어의 다른 컴포넌트
    protected Rigidbody rigidbody;
    protected Animator animator;
    private PlayerInput playerInput;
    // 플레이어 개인 속성
    public string nickName = "Kieeek";
    public float speed = 5f;
    public float rotateSpeed = 12f;
    public int potionCount = 5;
    protected Vector3 movement;
    
    // UI
    public GameObject ui;
    public Button[] buttons = new Button[6];
    public Slider slider; // 체력 표현

    // Network
    protected PhotonView pv;

    // 플레이어 기능 사용 가능여부
    protected bool attackCoolTime = false;
    protected bool avoidCoolTime = false;
    protected bool skillCoolTime = false;
    protected bool healCoolTime = false;
    protected bool shieldCoolTime = false;
    // Effect
    public GameObject ShieldPrefab;
    public GameObject AttackPrefab;
    public GameObject SkillPrefab;
    public GameObject AvoidPrefab;

    // 플레이어 객체 시작 및 종료
    virtual protected void Start()
    {
        // 플레이어의 다른 컴포넌트
        rigidbody = GetComponent<Rigidbody>();
        animator = GetComponent<Animator>();
        playerInput = GetComponent<PlayerInput>();
        playerSkin = GetComponentInChildren<SkinnedMeshRenderer>();

        // UI
        ui = GetComponentInChildren<Canvas>().gameObject;
        if (!photonView.IsMine)
            ui.SetActive(false);
        // Network
        pv = GetComponent<PhotonView>();

        // 플레이어 설정
        GameManager.players.Add(gameObject);
    }
    public void OnDestroy()
    {
        GameManager.players.Remove(gameObject);
    }
    // 움직임
    virtual protected void FixedUpdate()
    {
        // 로컬 플레이어만 위치 및 회전 변경 가능
        if(!photonView.IsMine)
        {
            return;
        }
        // 이동 불가 상태들
        if (animator.GetBool("Hitted") || animator.GetInteger("AttackBehavior") != 0 || animator.GetBool("isAvoid"))
        {
            return;
        }
        // 이동 및 회전
        if (playerInput.horizontalMove != 0 || playerInput.verticalMove != 0)
        {
            Run();
            Turn();
            // Animation
            animator.SetBool("isAvoid", false);
            animator.SetBool("isIdle", false);
        }
        else
        {
            // Animation
            animator.SetInteger("AttackBehavior", 0);
            animator.SetBool("isAvoid", false);
            animator.SetBool("isIdle", true);
            animator.SetBool("Hitted", false);
        }
    }
    // 행동 및 프레임 당 해야할 작업
    protected void Update()
    {
        // 로컬 플레이어만 행동 가능
        if (!photonView.IsMine)
        {
            return;
        }
        // 행동 불가 상태들
        if (animator.GetBool("Hitted") || animator.GetInteger("AttackBehavior") != 0 || animator.GetBool("isAvoid"))
        {
            return;
        }
        switch (playerInput.keyboardInput)
        {
            case PlayerInput.keyboard.attack:
                if(!attackCoolTime)
                {
                    animator.SetInteger("AttackBehavior", 1);
                    pv.RPC("StartAttack", RpcTarget.All);
                }
                break;
            case PlayerInput.keyboard.skill:
                if (!skillCoolTime)
                {
                    animator.SetInteger("AttackBehavior", 2);
                    pv.RPC("StartSkill", RpcTarget.All);
                }
                break;
            case PlayerInput.keyboard.heal:
                if (!healCoolTime)
                {
                    animator.SetInteger("AttackBehavior", 3);
                    pv.RPC("StartHeal", RpcTarget.All);
                } 
                break;
            case PlayerInput.keyboard.shield:
                if (!shieldCoolTime)
                {
                    animator.SetInteger("AttackBehavior", 4);
                    pv.RPC("StartShield", RpcTarget.All);
                }
                break;
            case PlayerInput.keyboard.potion:
                StartPotion();
                break;
            case PlayerInput.keyboard.avoid:
                if (!avoidCoolTime)
                {
                    animator.SetBool("isAvoid", true);
                    pv.RPC("StartAvoid", RpcTarget.All);
                }
                break;
            case PlayerInput.keyboard.nothing:
                animator.SetInteger("AttackBehavior", 0);
                break;
        }
    }

    // 움직임 함수들
    private void Run()
    {
        movement.Set(playerInput.horizontalMove - playerInput.verticalMove, 0, playerInput.verticalMove + playerInput.horizontalMove);
        movement = movement.normalized * speed * Time.deltaTime;
        rigidbody.MovePosition(transform.position + movement);
    }
    virtual protected void Turn()
    {
        Quaternion newRotation = Quaternion.LookRotation(movement);
        rigidbody.rotation = Quaternion.Slerp(rigidbody.rotation, newRotation, rotateSpeed * Time.deltaTime * 5);
    }
    // 행동 함수들
    [PunRPC]
    virtual protected void StartAttack()
    {
        attackCoolTime = true;
    }
    [PunRPC]
    virtual protected void StartSkill()
    {
        skillCoolTime = true;
        buttons[1].interactable = false;

    }
    [PunRPC]
    virtual protected void StartHeal()
    {
        healCoolTime = true;
        buttons[2].interactable = false;
    }
    [PunRPC]
    virtual protected void StartShield()
    {
        shieldCoolTime = true;
        buttons[3].interactable = false;

        GameObject effect = Instantiate(ShieldPrefab, transform.position + transform.forward + new Vector3(0, 2, 0), Quaternion.Euler(new Vector3(0, transform.eulerAngles.y, 0)));
        Destroy(effect, 1f);
        Invoke("EndShield", 1.3f);
    }
    protected void StartPotion()
    {
        if (potionCount > 0)
        {
            pv.RPC("RestoreHealth", RpcTarget.MasterClient, (float)70);
            potionCount--;
            Text text = buttons[4].GetComponentInChildren<Text>();
            text.text = potionCount.ToString();
        }
    }
    [PunRPC]
    virtual protected void StartAvoid()
    {
        avoidCoolTime = true;
        buttons[5].interactable = false;
    }
    virtual protected void EndAttack()
    {
        animator.SetInteger("AttackBehavior", 0);
        attackCoolTime = false;
        animator.SetBool("Hitted", false);
    }
    virtual protected void EndSkill()
    {
        skillCoolTime = false;
        buttons[1].interactable = true;
    }
    virtual protected void EndHeal()
    {
        healCoolTime = false;
        buttons[2].interactable = true;
    }
    protected void EndShield()
    {
        animator.SetInteger("AttackBehavior", 0);
        shieldCoolTime = false;
        buttons[3].interactable = true;
    }
    virtual protected void EndAvoid()
    {
        avoidCoolTime = false;
        buttons[5].interactable = true;
    }
    [PunRPC]
    public void OnStiffness()
    {
        animator.SetBool("Hitted", true);
    }
    // 죽었을 때 서서히 사라지는 효과 관련
    protected SkinnedMeshRenderer playerSkin;
    public enum BlendMode
    {
        Opaque,
        Cutout,
        Fade,
        Transparent
    };
    override public void Die()
    {
        base.Die();

        animator.SetInteger("isDead", 1);
        ChangeRenderMode(playerSkin.materials[0], BlendMode.Transparent);
        StartCoroutine("FadeOut");
        StartCoroutine(GameManager.gameManager.ShowContinew());
    }
    IEnumerator FadeOut()
    {
        yield return new WaitForSeconds(3.0f);
        for (float i = 1f; i >= 0; i -= 0.01f)
        {
            Color color = new Vector4(1, 1, 1, i);
            playerSkin.materials[0].color = color;
            yield return null;
        }
        gameObject.SetActive(false);
    }
    public static void ChangeRenderMode(Material standardShaderMaterial, BlendMode blendMode)
    {
        switch (blendMode)
        {
            case BlendMode.Transparent:
                standardShaderMaterial.SetInt("_SrcBlend", (int)UnityEngine.Rendering.BlendMode.One);
                standardShaderMaterial.SetInt("_DstBlend", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha);
                standardShaderMaterial.SetInt("_ZWrite", 0);
                standardShaderMaterial.DisableKeyword("_ALPHATEST_ON");
                standardShaderMaterial.DisableKeyword("_ALPHABLEND_ON");
                standardShaderMaterial.EnableKeyword("_ALPHAPREMULTIPLY_ON");
                standardShaderMaterial.renderQueue = 3000;
                break;
        }
    }

    // 부가적인 함수들
    [PunRPC]
    public void RPC_Coroutine(string function, object[] objects)
    {
        StartCoroutine(function, objects);
    }
    override public void OnPhotonSerializeView(PhotonStream stream, PhotonMessageInfo info)
    {
        base.OnPhotonSerializeView(stream, info);

        if (stream.IsWriting)
        {
            stream.SendNext(nickName);
        }
        else
        {
            nickName = (string)stream.ReceiveNext();
        }
    }
}

