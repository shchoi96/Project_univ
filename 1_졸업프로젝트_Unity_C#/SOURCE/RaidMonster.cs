using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.UI;
using Photon.Pun;
using BehaviorDesigner.Runtime;
using BehaviorDesigner.Runtime.Tasks;

public class RaidMonster : LivingEntity, IPunObservable
{
    // Monster Property
    public int currentPhase = 1;
    private double stiffness;          // 경직 상태
    public float damage;            // Monster Damage
    private bool isRotate = false;      // rotate in Attack
    private bool specialAttack = false;
    private bool shouting = false;
    public bool attack { get; set; } = false;

    // 사거리
    public float searchDist { get; protected set; } = 25.0f;    // 탐색 Distance
    public float farAtkDist { get; protected set; } = 12.0f;    // 원거리 공격 Distance
    public float attackDist { get; protected set; } = 7.0f;     // 공격 Distance
    public float dist { get; protected set; }

    // 공격 속성
    public GameObject targetPlayer;     // 공격 대상
    List<PlayerAp> playersData;         // 플레이어와의 관계

    public float skill_cooltime1 { get; set; } = 0;
    public float skill_cooltime2 { get; set; } = 0;
    public bool[] skill { get; set; } = { false, false };

    // Animation
    protected Animator animator;
    private GameObject hammer;

    // 공격 장판
    private GameObject range_atk1;
    private GameObject range_atk2;
    private GameObject range_atk3;
    private GameObject range_atk4;
    private GameObject range_atk5;
    private GameObject rect_range;
    private bool ranging = false;
    public float rangeTime;

    // effect
    public GameObject rockPrefab;
    public GameObject rockDummy;
    public GameObject icePrefab;
    public GameObject HammerEffectPrefab;
    public GameObject groundEffect;
    public GameObject rollingEffect;
    public GameObject swingEffect;
    private GameObject bossFoot;
    private GameObject bossHand;
    private GameObject LightPosition;
    public GameObject bossLight;

    //Sounds
    private AudioSource musicPlayer;
    public AudioClip scream;
    public AudioClip takeDownSound;
    public AudioClip SwingSound;
    public AudioClip rushSound;
    public AudioClip monsterInPainSound;

    // UI
    public Slider slider;

    // Navigation & Moving
    private NavMeshAgent nvAgent;       // 이동 AI

    // Network
    private bool currFindPlayer = false;
    private int currAttackBehavior = 0;
    public bool isMasterClient = false;
    private PhotonView pv;

    // Initiate Functions
    public override void OnEnable()
    {
        base.OnEnable();
        slider.maxValue = InitHealth; // 체력바 초기 설정
        slider.value = health;
        GameManager.monster = gameObject;
    }
    void Start()
    {
        //Use this for initialization.
        hammer = GameObject.FindGameObjectWithTag("BossWeapon");
        nvAgent = gameObject.GetComponent<NavMeshAgent>();
        animator = gameObject.GetComponent<Animator>();
        bossFoot = GameObject.FindGameObjectWithTag("BossFoot");
        bossHand = GameObject.FindGameObjectWithTag("boss");
        musicPlayer = gameObject.GetComponent<AudioSource>();
        LightPosition = GameObject.FindGameObjectWithTag("LightPosition");
        rockDummy.SetActive(false);
        stiffness = 0;
        // 공격 범위
        range_atk1 = GameObject.Find("Cyc/atk1_range");
        range_atk2 = GameObject.Find("Cyc/atk2_range");
        range_atk3 = GameObject.Find("Cyc/atk3_range");
        range_atk4 = GameObject.Find("Cyc/atk4_range");
        range_atk5 = GameObject.Find("Cyc/atk5_range");
        rect_range = GameObject.Find("Cyc/rect_range");
        range_atk1.SetActive(false);
        range_atk2.SetActive(false);
        range_atk3.SetActive(false);
        range_atk4.SetActive(false);
        range_atk5.SetActive(false);
        rect_range.SetActive(false);
        // Network
        pv = gameObject.GetPhotonView();

        playersData = new List<PlayerAp>();
        GetComponentInParent<BehaviorTree>().EnableBehavior();
        StartCoroutine(CheckState());
    }
    private IEnumerator CheckState()
    {
        while (!dead) /* not Dead state */
        {
            PhotonNetwork.SetMasterClient(PhotonNetwork.MasterClient);
            yield return new WaitForSeconds(0.5f);
            isMasterClient = PhotonNetwork.IsMasterClient;
        }
    }
    // 움직임
    [PunRPC]
    public void Trace()
    {
        if (dist < searchDist)
        {
            nvAgent.destination = targetPlayer.transform.position;
            animator.SetBool("findPlayer", true);
        }
        attack = false;
    }
    [PunRPC]
    public void Idle()
    {
        animator.SetBool("findPlayer", false);
    }

    // 페이즈 변화
    [PunRPC]
    public void ChangePhase2()
    {
        currentPhase = 2;
        animator.SetInteger("Phase", 2);
        if (PhotonNetwork.IsMasterClient)
            SpecialAtk1();
    }
    [PunRPC]
    public void ChangePhase3()
    {
        currentPhase = 3;
        animator.SetInteger("Phase", 3);
        AnimSpeedFast();
        if (PhotonNetwork.IsMasterClient)
            SpecialAtk1();
    }
    // 애니메이션 전체 속도 조절
    public void AnimSpeedFast()
    {
        if (currentPhase == 3)
            animator.speed = 1.2f;
        else
            animator.speed = 1f;
    }
    public void AnimSpeedLow()
    {
        animator.speed = 1f;
    }
    // targetPlayer 선정 함수
    public void SearchPlayer()
    {
        targetPlayer = SelectPlayer();
        pv.RPC("SetTargetPlayer", RpcTarget.Others, targetPlayer.GetComponent<PlayerCharacter>().nickName);
        if (targetPlayer != null)
        {
            dist = Vector3.Distance(targetPlayer.transform.position, transform.position);
        }
    }
    private GameObject SelectPlayer()
    {
        playersData.Clear();
        foreach (GameObject player in GameManager.players)
        {
            playersData.Add(new PlayerAp(player));
        }

        float totalAP = 0; // 플레이어들의 AP 총 합계
        // 플레이어 별 어그로 수치 계산
        foreach (PlayerAp player in playersData)
        {
            player.aggroPoint = 0;
            player.distance = Vector3.Distance(player.player.transform.position, transform.position);

            if (specialAttack)
                player.aggroPoint += player.distance * 30; // 협동 패턴을 위한 타겟 선정
            else
                player.aggroPoint += 160 / player.distance; // 거리가 멀수록 점수가 낮음
            // 힐러(위자드)일 경우 추가 점수
            if (player.IsHealer)
                player.aggroPoint += 20;
            totalAP += player.aggroPoint;
        }
        if (specialAttack)
        {
            float temp = playersData[0].aggroPoint;
            int num = 0;
            for(int i=0;i<playersData.Count;i++)
            {
                if(playersData[i].aggroPoint > temp)
                {
                    temp = playersData[i].aggroPoint;
                    num = i;
                }
            }
            return playersData[num].player;
        }
        else
        {
            float rand = Random.Range(0, totalAP);
            float sum = 0;
            // 0 - totalAP 사이의 랜덤 값을 통해 플레이어 선택 ( 플레이어는 totalAP 의 지분을 가짐 )
            foreach (PlayerAp player in playersData)
            {
                sum += player.aggroPoint;
                if (rand < sum)
                {
                    return player.player;
                }
            }
        }
        
        return playersData[0].player;
    }
    public class PlayerAp
    {
        // 어그로 수치 요소
        public GameObject player;
        public float distance; // 플레이어와의 거리
        public bool IsHealer = false;
        public float aggroPoint;

        public PlayerAp(GameObject play)
        {
            player = play;
            if (play.name == "wizard")
                IsHealer = true;
        }
    }

    /* 공격 함수들 */
    // 기본 공격1 : 내려찍기
    [PunRPC]
    public void BasicAttack1()
    { // 내려찍기
        animator.SetInteger("AttackBehavior", 1);
        StartCoroutine("StopBoss");
        StartCoroutine(RangeChargingRect(rect_range, 1.0f / animator.speed));
        Invoke("SetAttackFalse", 1f);
    }
    void HammerEffect()
    {
        StartCoroutine("HammerEffectRoutine");
    }
    IEnumerator HammerEffectRoutine()
    {
        playSound(takeDownSound, musicPlayer);
        var effect = Instantiate(HammerEffectPrefab, transform.position, Quaternion.Euler(new Vector3(0, transform.eulerAngles.y, 0)));
        yield return new WaitForSeconds(1.2f);
        Destroy(effect);
        yield break;
    }
    // 기본 공격2 : 휘두르기
    [PunRPC]
    public void BasicAttack2()
    {
        animator.SetInteger("AttackBehavior", 6);
        StartCoroutine("StopBoss");
        StartCoroutine(RangeCharging(range_atk2, 1.1f / animator.speed, 180f, false));
        StartCoroutine("SwingAttackEffect");
        Invoke("SetAttackFalse", 1.4f);
    }
    IEnumerator SwingAttackEffect()
    {
        playSound(SwingSound, musicPlayer);
        yield return new WaitForSeconds(0.8f);
        var effect = Instantiate(swingEffect, bossHand.transform.position + new Vector3(0, 2.4f, 0), Quaternion.Euler(new Vector3(160, transform.eulerAngles.y - 90, 180)));
        yield return new WaitForSeconds(2.5f);
        Destroy(effect);
        yield break;

    }
    // 기본 공격3 : 돌던지기
    [PunRPC]
    public void BasicAttack3()
    {
        animator.SetInteger("AttackBehavior", 3);
        StartCoroutine("StopBoss");
        isRotate = false;
        RangeCharging(range_atk3, 500f, targetPlayer.transform.position);
        StartCoroutine("InitRockParticle");
        Invoke("SetAttackFalse", 1.2f);
    }
    protected IEnumerator InitRockParticle()
    {
        rockDummy.SetActive(true);
        float trans = 2f;
        Vector3 pos = range_atk3.transform.position;
        yield return new WaitForSeconds(1.2f);
        rockDummy.SetActive(false);
        GameObject effect = Instantiate(rockPrefab, transform.position + new Vector3(0, 2, -1), Quaternion.identity);
        effect.GetComponent<Rigidbody>().AddForce((pos - effect.transform.position) * trans, ForceMode.Impulse);

        yield return new WaitForSeconds(1.5f);
        Destroy(effect);
    }
    // 기본 공격4 : 내려찍고 휘두르기 연속공격
    [PunRPC]
    public void BasicAttack4()
    {
        animator.SetInteger("AttackBehavior", 8);
        StartCoroutine(RangeChargingRect(rect_range, 1.0f / animator.speed));
        StartCoroutine("conTakeDownSwing");
        Invoke("SetAttackFalse", 1f);
    }
    IEnumerator conTakeDownSwing()
    {
        yield return new WaitForSeconds(2.3f);
        if (stiffness >= 2)
            yield break;
        else
        {
            StartCoroutine(RangeCharging(range_atk2, 1.1f / animator.speed, 180f, false));
            yield return StartCoroutine("SwingAttackEffect");
            yield break;
        }
    }

    /* 스킬 1 : 발 내려찍고 돌기 */
    [PunRPC]
    public void Skill1()
    { // 발 내려찍기
        animator.SetInteger("AttackBehavior", 7);
        StartCoroutine("StopBoss");
        isRotate = false;
        StartCoroutine(RangeCharging(range_atk4, 1.56f / animator.speed, 360f, true));
        StartCoroutine(Cooltime(1, 10));
        StartCoroutine("prevTakeDownEffect");
    }
    public void Skill1_1()
    { // 마구마구 돌기
        if (stiffness >= 2)
            return;
        else
        {
            StartCoroutine(TraceTarget(1f));
            StartCoroutine("tickAttack");
            StartCoroutine("nextTakeDownEffect");
            Invoke("SetAttackFalse", 1.2f);
        }
    }
    IEnumerator prevTakeDownEffect() // 발로 내려찍기
    {
        playSound(scream, musicPlayer);
        yield return new WaitForSeconds(1.5f);
        var effect1 = Instantiate(groundEffect, bossFoot.transform.position, Quaternion.Euler(new Vector3(0, transform.eulerAngles.y, 0)));
        yield return new WaitForSeconds(1.7f);
        Destroy(effect1);
    }
    IEnumerator tickAttack()
    {
        int i = 2;
        while (i > 0)
        {
            Collider[] colls = Physics.OverlapSphere(transform.position, 5.5f);
            Vector3 forward = transform.forward.normalized;
            foreach (Collider collider in colls)
            {
                AttackTrigger(collider);
            }
            i -= 1;
            yield return new WaitForSeconds(0.4f);
        }
    }
    IEnumerator nextTakeDownEffect() // 휘두르기(빙빙돌기) 
    {
        GameObject effect2 = Instantiate(rollingEffect, transform.position + new Vector3(0, 1.5f, 0f), Quaternion.Euler(new Vector3(0, transform.eulerAngles.y, 0)));
        effect2.transform.parent = gameObject.transform;
        yield return new WaitForSeconds(1.0f);
        Destroy(effect2);
        attack = false;
    }
    IEnumerator TraceTarget(float time)
    {
        nvAgent.isStopped = true;
        SearchPlayer();
        Vector3 distanceVector = (targetPlayer.transform.position - transform.position);
        while (time > 0)
        {
            time -= Time.deltaTime;
            transform.position += distanceVector * Time.deltaTime;          
            yield return null;
        }
        nvAgent.isStopped = false;
    }
    /* 스킬 2 : 세번 내려찍기 */
    [PunRPC]
    public void Skill2()
    {
        animator.SetInteger("AttackBehavior", 5);
        transform.rotation = Quaternion.LookRotation(targetPlayer.transform.position - transform.position);
        StartCoroutine(rotateToPlayer(0.62f));
        StartCoroutine("ThreeAttack");
        StartCoroutine(Cooltime(2, 10));
        Invoke("SetAttackFalse", 2f);
    }
    IEnumerator ThreeAttack()
    {
        animator.speed = 0.33f;
        StartCoroutine(RangeChargingRect(rect_range, 0.9f));
        yield return new WaitForSeconds(0.9f);
        AnimSpeedFast();
        if (stiffness >= 2)
            yield break;
        StartCoroutine(RangeChargingRect(rect_range, 0.42f)); 
        yield return new WaitForSeconds(0.42f);
        if (stiffness >= 2)
            yield break;
        StartCoroutine(RangeChargingRect(rect_range, 0.42f)); 
    }


    // 협동 패턴
    [PunRPC]
    public void SpecialAtk1()
    {
        specialAttack = true;
        attack = true;
        targetPlayer = SelectPlayer();
        pv.RPC("RPC_Coroutine", RpcTarget.All, "Rushing", targetPlayer.GetComponent<PlayerCharacter>().nickName);
    }
    public IEnumerator Rushing()
    {
        // 돌진 전 초기화
        nvAgent.isStopped = true;
        AnimSpeedLow();
        shouting = true;
        animator.SetBool("isRushed", true);
        animator.SetBool("isRushed2", true);
        stiffness = 0;

        transform.LookAt(targetPlayer.transform);
        // light 생성 및 sound 설정
        playSound(rushSound, musicPlayer);
        GameObject light = Instantiate(bossLight, transform.position + new Vector3(0, 5f, 0), Quaternion.Euler(new Vector3(0, transform.eulerAngles.y, 0)));
        light.transform.LookAt(targetPlayer.transform);
        light.GetComponent<Light>().range = 20;

        // 타겟 플레이어 이동 제약 및 얼음에 가두기
        targetPlayer.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezePosition | RigidbodyConstraints.FreezeRotation;
        GameObject ice = Instantiate(icePrefab, targetPlayer.transform.position, Quaternion.identity);

        // SystemMessage
        StartCoroutine(GameManager.gameManager.SystemMessage(targetPlayer.GetComponent<PlayerCharacter>().nickName + " 을 주시하고 있습니다"));

        //샤우팅 애니메이션 끝날 때 대기 후 1초 뒤 돌진
        animator.speed = 0.5f;
        while (shouting)
            yield return null;
        AnimSpeedLow();
        Destroy(light);

        //--돌진 시작--
        Vector3 destination = targetPlayer.transform.position - transform.position; /* boss - target player vector */

        while (Vector3.Distance(targetPlayer.transform.position, transform.position) > 1f)
        {
            if (stiffness >= 2)
                break;
            transform.position += destination * Time.deltaTime;       
            yield return null;
        }
        animator.SetBool("isRushed2", false);
        if (stiffness < 1)
        {
            //--타격--
            if (PhotonNetwork.IsMasterClient) 
            {
                PhotonView pv = targetPlayer.GetPhotonView();
                pv.RPC("OnDamage", RpcTarget.MasterClient, 60f);
            }
        }
        else if (stiffness == 1)
        {
            //--타격--
            if (PhotonNetwork.IsMasterClient)
            {
                PhotonView pv = targetPlayer.GetPhotonView();
                pv.RPC("OnDamage", RpcTarget.MasterClient, 30f);
            }
        } // 누적 데미지가 크면 타격 스킵
        //--상태 초기화--
        IceBreak(ice);
        targetPlayer.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
        animator.SetBool("isRushed", false);
        specialAttack = false;
        nvAgent.isStopped = false;
        yield return new WaitForSeconds(3f);
        Destroy(ice);
        AnimSpeedFast();
        Invoke("SetAttackFalse", 0.2f);
    }

    // 레이드 몬스터 죽음 관련 변수 및 함수
    private SkinnedMeshRenderer bossSkin;
    private SkinnedMeshRenderer weaponSkin;
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
        nvAgent.isStopped = true;
        animator.SetBool("isDead", true);
    }
    void DestroyBoss()
    {
        bossSkin = GameObject.FindGameObjectWithTag("mesh").GetComponent<SkinnedMeshRenderer>();
        weaponSkin = GameObject.FindGameObjectWithTag("BossWeapon").GetComponent<SkinnedMeshRenderer>();
        ChangeRenderMode(bossSkin.materials[0], BlendMode.Transparent);
        ChangeRenderMode(bossSkin.materials[1], BlendMode.Transparent);
        ChangeRenderMode(weaponSkin.material, BlendMode.Transparent);
        StartCoroutine("FadeOut");
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
    IEnumerator FadeOut()
    {
        for (float i = 1f; i >= 0; i -= 0.1f)
        {
            Color color = new Vector4(1, 1, 1, i);
            bossSkin.materials[0].color = color;
            bossSkin.materials[1].color = color;
            weaponSkin.material.color = color;
            yield return null;
        }
        Destroy(GameObject.FindGameObjectWithTag("boss"));
    }
    // 애니메이션 관련 함수
    public void resumeBoss()
    {
        nvAgent.isStopped = false;
        animator.SetInteger("AttackBehavior", 0);
    }

    IEnumerator StopBoss()
    {
        nvAgent.isStopped = true;
        nvAgent.destination = transform.position;

        StartCoroutine(rotateToPlayer(10f));
        StartCoroutine("RotationFalse");
        yield break;
    }
    IEnumerator rotateToPlayer(float speed)
    {
        isRotate = true;
        while (isRotate)
        {
            Vector3 toPlayer = targetPlayer.transform.position - transform.position; // boss to player vector
            toPlayer = new Vector3(toPlayer.x, 0, toPlayer.z);
            float step = speed * Time.deltaTime;
            Vector3 rotatePlayer = Vector3.RotateTowards(transform.forward, toPlayer, step, 0f);
            transform.rotation = Quaternion.LookRotation(rotatePlayer);
            yield return null;
        }
    }
    IEnumerator RotationFalse()
    {
        yield return new WaitForSeconds(0.5f);
        isRotate = false;
        yield break;
    }

    // 공격 적용 함수
    [PunRPC]
    public override void OnDamage(float damage)
    {
        base.OnDamage(damage);
        slider.value = health;
    }
    public void AttackTrigger(Collider other)
    {
        if (other.tag == "Player" && !animator.GetCurrentAnimatorStateInfo(0).IsName("Run") && !animator.GetCurrentAnimatorStateInfo(0).IsName("Idle"))
        {
            IDamageable target = other.GetComponentInParent<IDamageable>();
            if (target != null && PhotonNetwork.IsMasterClient)
            {
                PhotonView pv = other.gameObject.GetPhotonView();
                pv.RPC("OnDamage", RpcTarget.MasterClient, damage);
            }
        }
    }
    public void AbnormalStateTrigger(Collider other)
    {
        if (other.tag == "Player")
        {
            IDamageable target = other.GetComponentInParent<IDamageable>();
            if (target != null && PhotonNetwork.IsMasterClient)
            {
                PhotonView pv = other.GetComponentInParent<PhotonView>();
                pv.RPC("OnStiffness", RpcTarget.All);
            }
        }
    }
    [PunRPC]
    public void OnStiffness(double count)
    {
        if (shouting)
            return;
        stiffness += count;
        if (stiffness >= 2)
        {
            animator.SetBool("isSuffered", true);
            StartCoroutine("SufferedOff");
        }
    }
    IEnumerator SufferedOff()
    {
        yield return new WaitForEndOfFrame();
        playSound(monsterInPainSound, musicPlayer);
        animator.SetBool("isSuffered", false);
        yield return new WaitForSeconds(3.0f);
        stiffness = 0;
        resumeBoss();
        yield break;
    }

    /*   부가적인 함수들  */
    public void RPC(string method)
    {
        pv.RPC(method, RpcTarget.All);
    }
    [PunRPC]
    public void RPC_Coroutine(string function, string name)
    {
        SetTargetPlayer(name);
        StartCoroutine(function);
    }
    [PunRPC]
    public void SetTargetPlayer(string targetName)
    {
        foreach (GameObject otherPlayer in GameManager.players)
        {
            if (otherPlayer.GetComponent<PlayerCharacter>().nickName == targetName)
            {
                targetPlayer = otherPlayer;
            }
        }
    }
    public static void playSound(AudioClip clip, AudioSource audioPlayer)
    {
        audioPlayer.Stop();
        audioPlayer.clip = clip;
        audioPlayer.loop = false;
        audioPlayer.time = 0;
        audioPlayer.Play();
    }
    private void IceBreak(GameObject ice)
    {
        ice.transform.Find("testCrystal (2)/testCrystal").gameObject.SetActive(false);
        ice.transform.Find("testCrystal (2)/testCrystalAnim").gameObject.SetActive(true);
        ice.transform.Find("testCrystal (2)/Box001").gameObject.SetActive(false);
        ice.transform.Find("Audio/Explosion3").gameObject.SetActive(true);
    }
    public void EndShouting()
    {
        shouting = false;
    }
    // 스킬 쿨타임    
    private IEnumerator Cooltime(int skillNum, float time)
    {
        if (skillNum == 1)
        {
            skill_cooltime1 = time;
            while (skill_cooltime1 > 0)
            {
                skill_cooltime1 -= Time.deltaTime;
                yield return null;
            }
            skill_cooltime1 = 0;
        }
        else if (skillNum == 2)
        {
            skill_cooltime2 = time;
            while (skill_cooltime2 > 0)
            {
                skill_cooltime2 -= Time.deltaTime;
                yield return null;
            }
            skill_cooltime2 = 0;
        }
    }
    // 공격 범위 관련 함수
    IEnumerator RangeCharging(GameObject range, float time_max, float angle_max, bool abnormal) // 원형 범위
    { // range : 범위 오브젝트   time_max : 캐스팅 완료 시간   angle : 각도   abnormal : 상태이상여부
        range.SetActive(true);
        ranging = true;
        float time = Time.time;
        float lerp;
        rangeTime = time_max;
        CircularSectorMeshRenderer script = GameObject.Find(range.name + "/sub_range").GetComponent<CircularSectorMeshRenderer>();
        float max = script.radius;
        script.radius = 0;
        while (rangeTime > Time.time - time) // time_max->rangeTime
        {
            lerp = Mathf.Clamp01((Time.time - time) / time_max);
            script.radius = Mathf.Lerp(0, max, lerp);
            yield return new WaitForFixedUpdate();
        }
        Collider[] colls = Physics.OverlapSphere(range.transform.position, max * 0.68f);
        Vector3 forward = transform.forward.normalized;
        foreach (Collider collider in colls)
        { // angle_max == 360 : 원, 보다 작으면 부채꼴
            float angle = Vector3.Angle(forward, collider.GetComponentInParent<Transform>().position - transform.position);
            if (angle <= angle_max / 2 && PhotonNetwork.IsMasterClient)
            {
                AttackTrigger(collider);
                if (abnormal) AbnormalStateTrigger(collider);
            }
        }
        ranging = false;
        range.SetActive(false);

    }
    void RangeCharging(GameObject range, float time_max, Vector3 position)
    {
        range.transform.position = position;
        StartCoroutine(RangeCharging(range, time_max, 360f, false));
    }
    IEnumerator RangeChargingRect(GameObject range, float time_max) // 사각형 범위
    {
        range.SetActive(true);
        ranging = true;
        float time = Time.time;
        float lerp;
        GameObject sub_range = GameObject.Find(range.name + "/sub_range");
        Vector3 max_scale = sub_range.transform.localScale;
        Vector3 max_position = sub_range.transform.localPosition;
        sub_range.transform.localScale = new Vector3(1, 1, 0);
        sub_range.transform.localPosition = new Vector3(0, 0, 0.43f);
        while (time_max > Time.time - time)
        {
            lerp = Mathf.Clamp01((Time.time - time) / time_max);
            sub_range.transform.localScale = Vector3.Lerp(new Vector3(1, 1, 0), max_scale, lerp);
            sub_range.transform.localPosition = Vector3.Lerp(new Vector3(0, 0, 0.43f), max_position, lerp);
            yield return new WaitForFixedUpdate();
        }

        Collider[] colls = Physics.OverlapBox(range.transform.position, range.transform.localScale / 8, range.transform.rotation);
        foreach (Collider collider in colls)
        {
            AttackTrigger(collider);
        }
        ranging = false;
        range.SetActive(false);
    }
    void RangeActiveFalse()
    {
        range_atk1.SetActive(false);
        range_atk2.SetActive(false);
        range_atk3.SetActive(false);
        range_atk4.SetActive(false);
        range_atk5.SetActive(false);
        rect_range.SetActive(false);
    }
    void SetAttackFalse()
    {
        attack = false;
    }
}